#include "heap.h"

extern u64 _start_brk;
extern u64 _end_brk;

struct block *heap_head;

static void colapse_free_blocks(void)
{
  struct list_head *pos;
  list_for_each(pos, &heap_head->head)
  {
    struct block *block = (struct block *) pos;
    struct block *next = (struct block *) block->head.next;
    if(next->free && block->free)
    {
      block->size += next->size + sizeof(struct block);
      list_del(&next->head);
    }
  }
}

static struct block *search_first_free_block(size_t size)
{
  struct block *p = (struct block *) heap_head->head.next;
  struct list_head *pos = 0;
  list_for_each(pos, &heap_head->head)
  {
    p = (struct block *) pos;
    if(p->free)
    {
      if(!list_is_head(pos->next, &heap_head->head))
      {
        if(size <= p->size + sizeof(struct block))
          break;
      }
      else
      {
        if(size <= p->size)
          break;
      }
    }    
  }

  if(!list_is_head(&p->head, &heap_head->head))
    return p;
  
  return 0;
}

void *early_malloc(size_t size)
{
  if(!size)
    return 0;

  struct block *p = search_first_free_block(size);
  if(p)
  {
    if(p->size != size)
    {
      struct block *q = (struct block *) ((void *) p + size + sizeof(struct block));

      q->size = p->size - size - sizeof(struct block);
      p->size = size;
      q->free = 1;
      list_add(&q->head, &p->head);
    }
    
    p->free = 0;

    ++p;
  }

  return p;
}

void early_free(void *ptr)
{
  if(ptr)
  {
    struct block *block = ptr;
    (--block)->free = 1;

    colapse_free_blocks();
  }
}

void early_heap_init(void)
{
  heap_head = (struct block *) &_start_brk;
  
  list_head_init(&heap_head->head);
  
  size_t size = &_end_brk - &_start_brk - sizeof(struct block);
  struct block *first_block = (struct block *) ((void *) heap_head + size + sizeof(struct block));
  first_block->size = size - sizeof(struct block);
  first_block->free = 1;

  list_add(&first_block->head, &heap_head->head);
}
