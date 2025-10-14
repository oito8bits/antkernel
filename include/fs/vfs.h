#ifndef _INCLUDE_VFS_
#define _INCLUDE_VFS_

#define VFS_TYPE_LENGTH 32
#define VFS_PATH_LENGTH 64

#include <ant/list.h>
#include <stddef.h>

struct file_descriptor
{
  _Bool free;
  int mountpoint_id;
  char *name;
  int read_offset;
  int write_offset;
  int file_size;
};

struct fs_ops
{
  char *fs_name;
  int (*open)(const char *, int);
  int (*close)(int);
  size_t (*read)(int, void *, size_t, size_t);
  size_t (*write)(int, void *, size_t, size_t);
};

struct mountpoint
{
  struct list_head head;
  char device[VFS_PATH_LENGTH];
  char type[VFS_TYPE_LENGTH];
  char target[VFS_PATH_LENGTH];
  struct fs_ops ops;
};

int vfs_open(const char *, int);
size_t vfs_read(int fd, void *buffer, size_t size);
size_t vfs_write(int fd, void *buffer, size_t size);
int vfs_close(int);
int vfs_mount(char *, char *, char *);
int get_new_file_descriptor(void);
int vfs_init(void);

#endif
