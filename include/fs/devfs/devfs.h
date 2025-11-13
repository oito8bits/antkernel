#ifndef _INCLUDE_DEVFS_
#define _INCLUDE_DEVFS_

#include <ant/list.h>
#include <fs/vfs.h>
#include <fs/mp.h>

struct dev
{
  struct list_head head;
  struct vfs_ops *ops;
};

struct vfs_ops *devfs_get_ops(void);
int devfs_open(struct vfs_fd *, const char *, int);
int devfs_close(struct vfs_fd *);
int devfs_mount(struct mountpoint *);
int devfs_umount(struct mountpoint *);
void devfs_register(struct dev *);
void devfs_unregister(struct dev *);
void devfs_init(void);

#endif
