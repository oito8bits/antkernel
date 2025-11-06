#include <fs/vfs.h>
#include <mm/heap.h>
#include <libk/string.h>
#include <fs/ramfs/ramfs.h>
#include <fs/mp.h>

struct vfs_fd file_descriptors[4096];

static int get_new_file_descriptor(void)
{
  size_t i;
  for(i = 0; i < 4096; i++)
  {
    if(file_descriptors[i].fs_fd < 0)
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
  if(mp->ops->open(path, flags) == NULL)
    return -1;

  file_descriptors[fd] =  (struct vfs_fd)
                          {.fs_fd = 0,
                          .ops = mp->ops};
  
  return fd;
}

size_t vfs_read(int fd, void *buffer, size_t size)
{
  return file_descriptors[fd].ops->read(&file_descriptors[fd], buffer, size);
}

size_t vfs_write(int fd, void *buffer, size_t size)
{
  return file_descriptors[fd].ops->write(&file_descriptors[fd], buffer, size);
}

int vfs_close(int fd)
{
  if(file_descriptors[fd].ops->close(&file_descriptors[fd]) == -1)
    return -1;

  file_descriptors[fd].fs_fd = -1;

  return 0;
}

int vfs_mount(char *device, char *target, char *fs_type)
{
  struct mountpoint *mp = mp_create(device, target, fs_type);
  
  mp->ops->mount(mp);
}

int vfs_umount(char *target)
{
  struct mountpoint *mp = mp_search(target);
  mp->ops->umount(mp);
}

int vfs_init(void)
{
  mp_init();

  size_t i = 0;
  for(i = 0; i < 4096; i++)
    file_descriptors[i].fs_fd = -1;

  ramfs_init();

  return 0;
}
