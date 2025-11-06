#include <fs/mp.h>
#include <fs/ramfs/ramfs.h>
#include <string.h>
#include <mm/heap.h>

struct mountpoint mountpoints;

static struct vfs_ops *get_ops(char *fs_type)
{
  /* 
   * TODO: I need implement a another way to get operations.
   * A linked list might be a good idea.
   */
  if(!strcmp(fs_type, "ramfs"))
  {
    struct vfs_ops *ops = ramfs_get_ops(); 
    return ramfs_get_ops();
  }
  return NULL;
}

struct mountpoint *mp_create(char *device, char *target, char *fs_type)
{
  struct mountpoint *new = heap_malloc(sizeof(struct mountpoint));
  strcpy(new->device, device);
  strcpy(new->target, target);
  new->ops = get_ops(fs_type);
  list_add(&new->head, &mountpoints.head);

  return new;
}

void mp_destroy(struct mountpoint *mountpoint)
{
  list_del(&mountpoint->head);
  heap_free(mountpoint);
}

struct mountpoint *mp_search(const char *path)
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

void mp_init(void)
{
  list_head_init(&mountpoints.head);
}
