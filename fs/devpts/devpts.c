#include <fs/devpts/devpts.h>
#include <fs/devfs/devfs.h>
#include <drivers/keyboard/ps2.h>
#include <fs/mp.h>
#include <kernel/console.h>
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
  console_write(buffer);
}

size_t devpts_read(struct vfs_fd *fd, void *buffer, size_t count)
{
  char *buffer_ch = buffer;
  size_t i;
  struct keyboard_event event;
  for(i = 0; i < count; i++)
  {
    keyboard_get_event(&event);
    buffer_ch[i] = event.key_char;
    console_put_char(buffer_ch[i]);

    if(buffer_ch[i] == '\n')
    {
      i++;
      break;
    }
  }

  return i;
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
