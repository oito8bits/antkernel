#ifndef _INCLUDE_LOCK_
#define _INCLUDE_LOCK_

#include <ant/types.h>

struct lock
{
  bool locked;
};

void lock_acquire(struct lock *);
void lock_release(struct lock *);

#endif
