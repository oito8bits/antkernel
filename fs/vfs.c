#include <fs/vfs.h>
#include <mm/heap.h>
#include <libk/string.h>
#include <fs/ramfs/ramfs.h>
#include <fs/devfs/devfs.h>
#include <fs/mp.h>

struct vfs_fd file_descriptors[4096];

static int get_new_file_descriptor(void)
{
  size_t i;
  for(i = 0; i < 4096; i++)
  {
    if(!file_descriptors[i].free)
      return i;
  }

  return -1;
}

int vfs_open(const char *path, int flags)
{
  struct mountpoint *mp = mp_search(path);
  if(mp == NULL)
    return -1;
  int fd = get_new_file_descriptor();
  
  file_descriptors[fd].mp = mp;
  
  if(mp->ops->open(&file_descriptors[fd], path, flags) < 0)
    return -1;
 
  file_descriptors[fd].free = 1;
  file_descriptors[fd].offset = 0;
  
  return fd;
}

size_t vfs_read(int fd, void *buffer, size_t size)
{
  size_t ret = file_descriptors[fd].mp->ops->read(&file_descriptors[fd], buffer, size);
  file_descriptors[fd].offset += ret;
  return ret;
}

size_t vfs_write(int fd, void *buffer, size_t size)
{
  size_t ret = file_descriptors[fd].mp->ops->write(&file_descriptors[fd], buffer, size);
  file_descriptors[fd].offset += ret;

  return ret;
}

long int vfs_lseek(int fd, long int offset, int whence)
{
  long int new_offset = -1;
  switch(whence)
  {
    case VFS_SEEK_SET:
      new_offset = offset;
      break;
    case VFS_SEEK_CUR:
      new_offset = file_descriptors[fd].offset + offset;
      break;
    case VFS_SEEK_END:
      new_offset = file_descriptors[fd].size + offset;
      break;
  }

  file_descriptors[fd].offset = new_offset;

  return new_offset;
}

int vfs_close(int fd)
{
  if(file_descriptors[fd].mp->ops->close(&file_descriptors[fd]) == -1)
    return -1;

  file_descriptors[fd].free = 1;

  return 0;
}

int vfs_mount(char *device, char *target, char *fs_type)
{
  struct mountpoint *mp = mp_create(device, target, fs_type);
  mp->dev_fd = vfs_open(device, 0); 
  mp->ops->mount(mp);
}

int vfs_umount(char *target)
{
  struct mountpoint *mp = mp_search(target);
  vfs_close(mp->dev_fd);
  mp->ops->umount(mp);
}

int vfs_init(void)
{
  mp_init();

  size_t i = 0;
  for(i = 0; i < 4096; i++)
    file_descriptors[i].free = 0;

  devfs_init();
  ramfs_init();

  return 0;
}
