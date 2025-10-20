#include <fs/ramfs/ramfs.h>
#include <fs/vfs.h>

struct fs_ops tar_ops =
{
  .fs_name = "ramfs",
  .open = ramfs_open,
  .close = ramfs_close,
  .read = ramfs_read,
  .write = ramfs_write
};

struct fs_ops *ramfs_get_ops(void)
{
  return &tar_ops;
}

int ramfs_open(const char *path, int flags)
{
}

size_t ramfs_write(int fd, void *buffer, size_t count)
{
}

size_t ramfs_read(int fd, void *buffer, size_t count)
{
}

int ramfs_close(int fd)
{
}

void ramfs_init(void)
{
}
