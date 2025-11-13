#ifndef _INCLUDE_RAMDISK_
#define _INCLUDE_RAMDISK_

#include <ant/types.h>
#include <fs/vfs.h>

size_t ramdisk_write(struct vfs_fd *, void *, size_t);
size_t ramdisk_read(struct vfs_fd *, void *, size_t);
void ramdisk_init(void);

#endif
