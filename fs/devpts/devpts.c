#include <fs/devpts/devpts.h>
#include <fs/devfs/devfs.h>
#include <fs/mp.h>
#include <fb/fb.h>

struct vfs_ops devpts_ops =
{
  .fs_name = "devpts",
  .write = devpts_write
};

struct dev devpts_dev;

size_t devpts_write(struct vfs_fd *fd, void *buffer, size_t count)
{
  fb_write(buffer);
}

struct vfs_ops *devpts_get_ops(void)
{
  return &devpts_ops;
}

void devpts_init(void)
{
  devpts_dev.ops = &devpts_ops;
  devfs_register(&devpts_dev);
  vfs_mount("", "/dev/pts/", "devpts");
}
