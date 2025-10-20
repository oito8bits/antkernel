#include <fs/vfs.h>
#include <mm/heap.h>
#include <libk/string.h>
#include <fs/ramfs/ramfs.h>

struct mountpoint mountpoints;
struct file_descriptor file_descriptors[4096];
extern struct fs_ops tar_ops;


__attribute__((noinline))
static struct fs_ops *get_ops(char *fs_type)
{
  /* 
   * TODO: I need implement a another way to get operations.
   * A linked list might be a good idea.
   */
  if(!strcmp(fs_type, "ramfs"))
  {
    struct fs_ops *ops = ramfs_get_ops(); 
    return ramfs_get_ops();
  }
  return NULL;
}

static void create_mountpoint(char *device, char *target, char *fs_type)
{
  struct mountpoint *new = heap_malloc(sizeof(struct mountpoint));
  strcpy(new->device, device);
  strcpy(new->target, target);
  new->ops = get_ops(fs_type);
  list_add(&new->head, &mountpoints.head);
}

static void destroy_mountpoint(struct mountpoint *mountpoint)
{
  list_del(&mountpoint->head);
  heap_free(mountpoint);
}

static struct mountpoint *get_mountpoint(const char *path)
{
  struct mountpoint *mp = NULL;
  char *mp_str = "";
  struct list_head *pos;
  list_for_each(pos, &mountpoints.head)
  {
    struct mountpoint *mp_pos = (struct mountpoint *) pos;
    if(!strncmp(mp_pos->target, path, strlen(mp_pos->target)))
    {
      if(strlen(mp_pos->target) > strlen(mp_str) &&
         path[strlen(mp_pos->target)] == '/')
      {
        mp = mp_pos;
        mp_str = mp->target;
      }
    }
  }

  return mp;
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
__attribute__((noinline)) int test(struct mountpoint *mp, const char *path, int flags)
{
  return mp->ops->open(path, flags);
}
int vfs_open(const char *path, int flags)
{
  struct mountpoint *mp = get_mountpoint(path);
  if(mp == NULL)
    return -1;
  return test(mp, path, flags);
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
