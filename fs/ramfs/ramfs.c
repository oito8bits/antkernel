#include <fs/ramfs/ramfs.h>
#include <fs/ramfs/ramdisk.h>
#include <fs/vfs.h>
#include <mm/vmm.h>

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

struct vfs_ops *ramfs_get_ops(void)
{
  return &tar_ops;
}

int ramfs_open(struct vfs_fd *fd, const char *path, int flags)
{
  vfs_open(fd->mp->device, 0);
}

size_t ramfs_write(struct vfs_fd *fd, void *buffer, size_t count)
{
}

size_t ramfs_read(struct vfs_fd *fd, void *buffer, size_t count)
{
}

int ramfs_close(struct vfs_fd *fd)
{
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
