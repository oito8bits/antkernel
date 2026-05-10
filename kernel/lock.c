#include <kernel/lock.h>

void lock_acquire(struct lock *lock)
{
  while(true)
  {
    if(lock->locked == false)
    {
      lock->locked = true;
      return;
    }
  }
}

void lock_release(struct lock *lock)
{
  lock->locked = false;
}
