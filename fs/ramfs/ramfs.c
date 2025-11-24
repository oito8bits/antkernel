#include <fs/ramfs/ramfs.h>
#include <fs/ramfs/ramdisk.h>
#include <fs/vfs.h>
#include <mm/vmm.h>
#include <mm/heap.h>
#include <libk/string.h>
#include <ant/align.h>

#define BLOCK_SIZE 512

struct vfs_ops tar_ops =
{
  .fs_name = "ramfs",
  .open = ramfs_open,
  .close = ramfs_close,
  .read = ramfs_read,
  .write = ramfs_write,
  .mount = ramfs_mount,
  .umount = ramfs_umount
};

struct ramfs_fd
{
  size_t absolute_offset;
};

static u64 octal_to_dec(char *source)
{
  u64 result = 0;
  size_t i;
  for(i = 0; source[i]; i++)
    result = result * 8 + source[i] - '0'; 

  return result;
} 

struct vfs_ops *ramfs_get_ops(void)
{
  return &tar_ops;
}

int ramfs_open(struct vfs_fd *fd, const char *path, int flags)
{
  int dev_fd = fd->mp->dev_fd;
  if(dev_fd < 0)
    return dev_fd;

  path++;

  size_t old_offset = vfs_lseek(dev_fd, 0, VFS_SEEK_CUR);
  vfs_lseek(dev_fd, 0, VFS_SEEK_SET);

  size_t name_len, path_len;
  struct ramfs_tar_header *header;
  struct ramfs_fd *ramfs_fd = heap_malloc(sizeof(struct ramfs_fd));
  char buffer[BLOCK_SIZE];
  do
  {
    vfs_read(dev_fd, buffer, BLOCK_SIZE);
    header = (struct ramfs_tar_header *) buffer;
    strcpy(header->name, path);
    
    name_len = strlen(header->name);
    path_len = strlen(path);
    
    size_t file_size = BLOCK_SIZE;
    if(!strncmp(header->name, path, name_len))
    {
      vfs_lseek(dev_fd, file_size, VFS_SEEK_CUR);
      continue;
    }
  } while(name_len != path_len);
  ramfs_fd->absolute_offset = vfs_lseek(dev_fd, 0, VFS_SEEK_CUR) - BLOCK_SIZE;
  fd->fs_file_info = ramfs_fd;
#include <libk/kprintf.h>
  kprintf("abosulte_offset: %li\n", ramfs_fd->absolute_offset);
  vfs_lseek(dev_fd, old_offset, VFS_SEEK_SET);

  return 0;
}

size_t ramfs_write(struct vfs_fd *fd, void *buffer, size_t count)
{
  int dev_fd = fd->mp->dev_fd;
  struct ramfs_fd *ramfs_fd = fd->fs_file_info;
  size_t old_offset = vfs_lseek(dev_fd, 0, VFS_SEEK_CUR);
  vfs_lseek(dev_fd, ramfs_fd->absolute_offset, VFS_SEEK_CUR);
  size_t size = vfs_write(dev_fd, buffer, count);
  vfs_lseek(dev_fd, old_offset, VFS_SEEK_SET);
  
  return size;
}

size_t ramfs_read(struct vfs_fd *fd, void *buffer, size_t count)
{
  int dev_fd = fd->mp->dev_fd;
  struct ramfs_fd *ramfs_fd = fd->fs_file_info;
  size_t old_offset = vfs_lseek(dev_fd, 0, VFS_SEEK_CUR);
  vfs_lseek(dev_fd, ramfs_fd->absolute_offset, VFS_SEEK_CUR);
  size_t size = vfs_read(dev_fd, buffer, count);
  vfs_lseek(dev_fd, old_offset, VFS_SEEK_SET);

  return size;
}

int ramfs_close(struct vfs_fd *fd)
{
  if(fd->fs_file_info)
    return 0;

  heap_free(fd->fs_file_info);

  return 0;
}

int ramfs_mount(struct mountpoint *mp)
{
}

int ramfs_umount(struct mountpoint *mp)
{
}

void ramfs_init(void)
{
  ramdisk_init();
}
