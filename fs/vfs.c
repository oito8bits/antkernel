#include <fs/vfs.h>
#include <mm/heap.h>
#include <libk/string.h>
#include <fs/ramfs/ramfs.h>

struct mountpoint mountpoints;
struct file_descriptor file_descriptors[4096];
extern struct fs_ops tar_ops;


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

static struct mountpoint *search_mountpoint(const char *path)
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

static int get_new_file_descriptor(void)
{
  size_t i;
  for(i = 0; i < 4096; i++)
  {
    if(!file_descriptors[i].fs_fd)
      return i;
  }

  return -1;
}

static struct mountpoint *get_fd_mountpoint(int fd)
{
  return file_descriptors[fd].mountpoint;
}

static int get_fd_fs_fd(int fd)
{
  return file_descriptors[fd].fs_fd;
}

int vfs_open(const char *path, int flags)
{
  struct mountpoint *mp = search_mountpoint(path);
  if(mp == NULL)
    return -1;

  int fs_fd = mp->ops->open(path, flags);
  if(fs_fd == -1)
    return -1;

  int fd = get_new_file_descriptor();
  file_descriptors[fd] =  (struct file_descriptor)
                          {.fs_fd = fs_fd,
                          .mountpoint = mp, 
                          .read_offset = 0,
                          .write_offset = 0};
  
  return fd;
}

size_t vfs_read(int fd, void *buffer, size_t size)
{
  struct mountpoint *mp = get_fd_mountpoint(fd);

  return mp->ops->read(get_fd_fs_fd(fd), buffer, size);
}

size_t vfs_write(int fd, void *buffer, size_t size)
{
  struct mountpoint *mp = get_fd_mountpoint(fd);

  return mp->ops->write(get_fd_fs_fd(fd), buffer, size);
}

int vfs_close(int fd)
{
  struct mountpoint *mp = get_fd_mountpoint(fd);
  if(mp->ops->close(get_fd_fs_fd(fd)) == -1)
    return -1;

  file_descriptors[fd].fs_fd = -1;

  return 0;
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
  
  size_t i = 0;
  for(i = 0; i < 4096; i++)
    file_descriptors[i].fs_fd = -1;

  return 0;
}
