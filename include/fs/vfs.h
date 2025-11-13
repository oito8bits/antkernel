#ifndef _INCLUDE_VFS_
#define _INCLUDE_VFS_

#include <ant/list.h>
#include <stddef.h>
#include <fs/mp.h>

struct vfs_fd
{
  int free;
  struct mountpoint *mp;
  void *fs_handler;
};

struct vfs_ops
{
  char *fs_name;
  int (*open)(struct vfs_fd *, const char *, int);
  int (*close)(struct vfs_fd *);
  size_t (*read)(struct vfs_fd *, void *, size_t);
  size_t (*write)(struct vfs_fd *, void *, size_t);
  int (*mount)(struct mountpoint *);
  int (*umount)(struct mountpoint *);
};

int vfs_open(const char *, int);
size_t vfs_read(int, void *, size_t);
size_t vfs_write(int, void *, size_t);
int vfs_close(int);
int vfs_mount(char *, char *, char *);
int vfs_init(void);

#endif
