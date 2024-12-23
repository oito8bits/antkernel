#ifndef _INCLUDE_HEAP_
#define _INCLUDE_HEAP_

#include <ant/list.h>
#include <ant/types.h>

struct block
{
  struct list_head head;
  size_t size;
  _Bool free;
};

void early_heap_init(void);
void *early_malloc(size_t);
void early_free(void *);
#endif
