#include <mm/heap.h>
#include <mm/vmm.h>
#include <libk/kprintf.h>
#include <ant/align.h>

static struct block heap_head;
extern u64 kernel_top_table;

void *heap_base;
size_t heap_size;

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

__attribute__((noinline))
static struct block *resize_heap(size_t size)
{
  void *heap_end = heap_base + heap_size;
  if(IS_ALIGN(size, PAGE_SIZE))
    size = ALIGNUP(size, PAGE_SIZE);

  vmm_map((struct table_entry *) &kernel_top_table, heap_end, size / PAGE_SIZE, BIT_PRESENT | BIT_WRITE);

  struct block *last_block = (struct block *) heap_head.head.prev;
  last_block->size += size;

  return last_block;
}

__attribute__((noinline))
static struct block *search_first_free_block(size_t size)
{
  struct block *p = 0;
  struct list_head *pos;
  list_for_each(pos, &heap_head.head)
  {
    p = (struct block *) pos;
    if(!p->free)
      continue;

    if(size <= p->size + sizeof(struct block))
      break;

    if(list_is_head(p->head.next, &heap_head.head))
    {
      p = resize_heap(size + sizeof(struct block));
      break;
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
  heap_base = (void *) KERNEL_HEAP_BASE;
  heap_size = UNIT_KiB(32); /* 16 KiB */
  void *end_addr = heap_base + heap_size;
  struct block *first_block = heap_base;

  vmm_map((struct table_entry *) &kernel_top_table,
          heap_base,
          heap_size / PAGE_SIZE,
          BIT_PRESENT | BIT_WRITE);

  first_block->size = heap_size - sizeof(struct block);
  first_block->free = 1;

  struct block *last_block = (struct block *) ((void *) first_block + heap_size
                             - sizeof(struct block));
  *last_block = (struct block) {0};
  
  list_head_init(&heap_head.head);

  list_add(&first_block->head, &heap_head.head);
}
