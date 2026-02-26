#ifndef _INCLUDE_DEVPTS_
#define _INCLUDE_DEVPTS_

#include <ant/types.h>
#include <fs/vfs.h>

struct vfs_ops *devpts_get_ops(void);
size_t devpts_write(struct vfs_fd *, void *, size_t);
void devpts_init(void);

#endif
