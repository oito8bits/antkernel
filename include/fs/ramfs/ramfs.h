#ifndef _INCLUDE_RAMFS_
#define _INCLUDE_RAMFS_

#include <ant/types.h>
#include <fs/mp.h>
#include <fs/vfs.h>

#define MAGIC "ustar"

struct ramfs_tar_header
{
  char name[100];
  char mode[8];
  char uid[8];
  char gid[8];
  char size[12];
  char mtime[12];
  char checksum[8];
  char type_flag;
  char link_name[100];
  char magic[6];
  char version[6];
  char uname[32];
  char gname[32];
  char dev_major[8];
  char dev_minor[8];
  char prefix[155];
};

struct vfs_ops *ramfs_get_ops(void);
int ramfs_open(struct vfs_fd *, const char *, int);
size_t ramfs_write(struct vfs_fd *, void *, size_t);
size_t ramfs_read(struct vfs_fd *, void *, size_t);
int ramfs_close(struct vfs_fd *);
int ramfs_mount(struct mountpoint *);
int ramfs_umount(struct mountpoint *);
void ramfs_init(void);

#endif
