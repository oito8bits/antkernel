#include <fs/vfs.h>

size_t write(int fd, const void *buf, size_t count)
{
  size_t ret;

  ret = vfs_write(fd, (void *) buf, count);

  return ret;
}
