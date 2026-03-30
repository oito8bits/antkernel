#include <fs/devpts/devpts.h>
#include <fs/devfs/devfs.h>
#include <drivers/keyboard/ps2.h>
#include <fs/mp.h>
#include <fb/fb.h>

struct vfs_ops devpts_ops =
{
  .fs_name = "devpts",
  .write = devpts_write,
  .read = devpts_read
};

struct dev devpts_dev;

size_t devpts_write(struct vfs_fd *fd, void *buffer, size_t count)
{
  fb_write(buffer);
}

size_t devpts_read(struct vfs_fd *fd, void *buffer, size_t count)
{
  char *buffer_ch = buffer;
  size_t i;
  for(i = 0; i < count; i++)
  {
    buffer_ch[i] = keyboard_get_char();
    fb_put_char(buffer_ch[i]);

    if(buffer_ch[i] == '\n')
      break;
  }
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
