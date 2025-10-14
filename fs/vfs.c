#include <fs/vfs.h>
#include <mm/heap.h>
#include <libk/string.h>

struct mountpoint mountpoints;
struct file_descriptor file_descriptors[4096];

static void create_mountpoint(char *device, char *target, char *fs_type)
{
  struct mountpoint *new = heap_malloc(sizeof(struct mountpoint));
  strcpy(new->device, device);
  strcpy(new->target, target);
  list_add(&new->head, &mountpoints.head);
}

static void destroy_mountpoint(struct mountpoint *mountpoint)
{
  list_del(&mountpoint->head);
  heap_free(mountpoint);
}

static struct mountpoint *get_mountpoint(const char *path)
{
  struct list_head *pos;
  list_for_each(pos, &mountpoints.head)
  {
    struct mountpoint *mp = (struct mountpoint *) pos;
    //if(!strncmp(mp->target, path, strlen(mp->target)))
      // todo
  }
}

int get_new_file_descriptor(void)
{
  size_t i;
  for(i = 0; i < 4096; i++)
  {
    if(!file_descriptors[i].free)
      return i;
  }

  return -1;
}

int vfs_open(const char *path, int flags)
{
  struct mountpoint *mp = get_mountpoint(path);
  if(mp == NULL)
    return -1;

  return mp->ops.open(path, flags);
}

size_t vfs_read(int fd, void *buffer, size_t size)
{
}

size_t vfs_write(int fd, void *buffer, size_t size)
{
}

int vfs_close(int fd)
{
}

int vfs_mount(char *device, char *target, char *fs_type)
{
  create_mountpoint(device, target, fs_type);
}

int vfs_umount(struct mountpoint *mountpoint)
{
  destroy_mountpoint(mountpoint);
}

int vfs_init(void)
{
  list_head_init(&mountpoints.head);

  return 0;
}
