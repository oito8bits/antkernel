#ifndef _INCLUDE_MP_
#define _INCLUDE_MP_

#include <ant/list.h>

struct mountpoint
{
  struct list_head head;
  char device[32];
  char type[32];
  char target[32];
  struct vfs_ops *ops;
  int dev_fd;
};

struct mountpoint *mp_create(char *, char *, char *);
void mp_destroy(struct mountpoint *);
struct mountpoint *mp_search(const char *);
void mp_init(void);

#endif
