#include <mm/pmm.h>

#include <mm/map.h>
#include <table_entry.h>
#include <libk/kprintf.h>
#include <libk/string.h>
#include <libk/stddef.h>
#include <mm/early_heap.h>
#include <mm/addr.h>

struct page *page_area;
u64 pages;
u64 max_order;

struct page *free_buddies;

static u32 get_page_idx(struct page *page)
{
  return page - page_area;
}

static struct page *get_buddy(struct page *page)
{
  u32 idx = get_page_idx(page);
  idx ^= 1 << page->order;
  return &page_area[idx];
}

void show_buddies(void)
{
  int i;
  struct list_head *node;
  for(i = max_order; i >= 0; i--)
  {
    list_for_each(node, &free_buddies[i].head)
    {
      struct page *page = (struct page *) node;
      kprintf("Node addr: 0x%lx, order: %li, buddy: 0x%lx\n", page, i, get_buddy(page));
    }
  }
}

static struct page *split_buddy(u32 order)
{
  struct page *page = NULL, *buddy;
  size_t i;
  for(i = max_order; order < i; i--)
  {
    if(list_empty(&free_buddies[i - 1].head) &&
      !list_empty(&free_buddies[i].head))
    {
      page = (struct page *) free_buddies[i].head.next;
      list_del(&page->head);
      list_add(&page->head, &free_buddies[i - 1].head);
      page->order--;
      buddy = get_buddy(page);
      buddy->order = page->order;
      list_add(&buddy->head, &page->head);
    }
  }

  return page;
}

struct page *pmm_alloc_order(u32 order)
{
  struct page *page;
  
  page = (struct page *) free_buddies[order].head.next;
  if(list_is_head(&page->head, &free_buddies[order].head))
    page = split_buddy(order);

  if(page != NULL)
  {
    list_del(&page->head); 
    page->head.next = NULL;
    page->head.prev = NULL;
  }

  return page;
}

void pmm_free_order(struct page *page)
{
  struct page *buddy;
  kprintf("max_order: %u\n", max_order);
  while(page->order < max_order)
  {
    buddy = get_buddy(page);

    if(buddy->head.next == NULL &&
       buddy->head.prev == NULL ||
       page->order != buddy->order)
      break;
    
    list_del(&buddy->head);
    if(buddy < page)
      page = buddy;
    page->order++;
  }

  list_add(&page->head, &free_buddies[page->order].head);
}

void pmm_init(struct boot_info *boot_info)
{
  pages = map_get_memory_pages(&boot_info->map);

  page_area = addr_phys_to_virt((void *) boot_info->kernel_entry) + boot_info->kernel_size;
  memset(page_area, 0, pages * sizeof(struct page));

  size_t i;
  for(i = 0; 1 << i < pages; i++);
  max_order = i;

  free_buddies = early_malloc((max_order + 1) * sizeof(struct page));
  memset(free_buddies, 0, max_order * sizeof(struct page));

  for(i = 0; i <= max_order; i++)
    list_head_init(&free_buddies[i].head);

  page_area->order = max_order;
  list_add(&page_area->head, &free_buddies[max_order].head);
}
