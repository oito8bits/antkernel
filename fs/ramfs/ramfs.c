#include <fs/tar.h>
#include <fs/vfs.h>

struct fs_ops tar_ops =
{
  .fs_name = "tar",
  .open = tar_open,
  .close = tar_close,
  .read = tar_read,
  .write = tar_write
};

int tar_open(char *path, int flags)
{
}

size_t tar_write(int fd, void *buffer, size_t count)
{
}

size_t tar_read(int fd, void *buffer, size_t count)
{
}

void tar_close(int fd)
{
}

void tar_init(void)
{
}
