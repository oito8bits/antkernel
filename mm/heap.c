#include <mm/heap.h>
#include <mm/vmm.h>
#include <libk/kprintf.h>

static struct block heap_head;
extern u64 kernel_top_table;

static void colapse_free_blocks(void)
{
  struct list_head *pos;
  list_for_each(pos, &heap_head.head)
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
  struct block *p = (struct block *) heap_head.head.next;
  struct list_head *pos = 0;
  list_for_each(pos, &heap_head.head)
  {
    p = (struct block *) pos;
    if(p->free)
    {
      if(!list_is_head(pos->next, &heap_head.head))
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

  if(!list_is_head(&p->head, &heap_head.head))
    return p;
  
  return 0;
}

void heap_show_blocks(void)
{
  struct block *p = (struct block *) heap_head.head.next;
  struct list_head *pos = 0;
  list_for_each(pos, &heap_head.head)
  {
    p = (struct block *) pos;
    kprintf("%li, free: %i, addr: %lx\n", p->size, p->free, p);
  }
}

void *heap_malloc(size_t size)
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

void heap_free(void *ptr)
{
  if(ptr)
  {
    struct block *block = ptr;
    (--block)->free = 1;

    colapse_free_blocks();
  }
}

void heap_init(void)
{
  void *start_addr = (void *) KERNEL_BASE;
  size_t size = 32 * (1 << 10); /* 16 KiB */
  void *end_addr = start_addr + size;
  struct block *first_block = start_addr;

  vmm_map((struct table_entry *) &kernel_top_table,
          start_addr,
          size / PAGE_SIZE,
          BIT_PRESENT | BIT_WRITE);

  first_block->size = size - sizeof(struct block) * 2;
  first_block->free = 1;

  struct block *last_block = (struct block *) ((void *) first_block + size
                             - sizeof(struct block));
  *last_block = (struct block) {0};
  
  list_head_init(&heap_head.head);

  list_add(&first_block->head, &heap_head.head);
  list_add(&last_block->head, &first_block->head);
}
