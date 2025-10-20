#ifndef _INCLUDE_RAMFS_
#define _INCLUDE_RAMFS_

#include <ant/types.h>

struct fs_ops *ramfs_get_ops(void);
int ramfs_open(const char *, int);
size_t ramfs_write(int, void *, size_t);
size_t ramfs_read(int, void *, size_t);
int ramfs_close(int);
void ramfs_init(void);

#endif
