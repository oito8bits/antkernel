#ifndef _INCLUDE_VFS_
#define _INCLUDE_VFS_

#include <ant/list.h>
#include <stddef.h>
#include <fs/mp.h>

struct vfs_fd
{
  int fs_fd;
  struct vfs_ops *ops;
};

struct vfs_ops
{
  char *fs_name;
  struct vfs_fd *(*open)(const char *, int);
  int (*close)(struct vfs_fd *);
  size_t (*read)(struct vfs_fd *, void *, size_t);
  size_t (*write)(struct vfs_fd *, void *, size_t);
  int (*mount)(struct mountpoint *);
  int (*umount)(struct mountpoint *);
};

int vfs_open(const char *, int);
size_t vfs_read(int fd, void *buffer, size_t size);
size_t vfs_write(int fd, void *buffer, size_t size);
int vfs_close(int);
int vfs_mount(char *, char *, char *);
int vfs_init(void);

#endif
