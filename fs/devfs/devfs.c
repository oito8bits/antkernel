#include <fs/devfs/devfs.h>
#include <libk/string.h>

static struct dev devs;

struct vfs_ops devfs_ops =
{
  .fs_name = "devfs",
  .open = devfs_open,
  .close = devfs_close,
  .mount = devfs_mount,
  .umount = devfs_umount
};

struct vfs_ops *devfs_get_ops(void)
{
  return &devfs_ops;
}

static const char *get_dev_name(const char *path)
{
  const char *dev_name;
  while(*path)
  {
    if(*path == '/')
      dev_name = path + 1;

    path++;
  }

  return dev_name;
}

int devfs_open(struct vfs_fd *fd, const char *path, int flags)
{
  struct list_head *pos;

  const char *dev_name = get_dev_name(path);
  list_for_each(pos, &devs.head)
  {
    struct dev *dev = (struct dev *) pos;
    if(!strcmp(dev->ops->fs_name, dev_name))
    {
      fd->mp->ops = dev->ops;
      return 0;
    }
  }

  return -1;
}

int devfs_close(struct vfs_fd *fd)
{
  return 0;
}

int devfs_mount(struct mountpoint *mp)
{
}

int devfs_umount(struct mountpoint *mp)
{
}

void devfs_register(struct dev *dev)
{
  list_add(&dev->head, &devs.head);
}

void devfs_unregister(struct dev *dev)
{
  list_del(&dev->head);
}

void devfs_init(void)
{
  list_head_init(&devs.head);
}
