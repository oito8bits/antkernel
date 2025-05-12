#include <mm/pmm.h>

#include <mm/map.h>
#include <table_entry.h>
#include <libk/kprintf.h>
#include <libk/string.h>
#include <mm/early_heap.h>
#include <mm/addr.h>

struct page *page_area;
u64 pages;
u64 max_order;

struct page *free_buddies;

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

  list_add(&page_area->head, &free_buddies[max_order].head);
}

static struct page *get_buddy(struct page *page, u32 order)
{
  return (struct page *) page + (1 << order);
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
      kprintf("Node addr: 0x%lx, order: %li\n", page, i);
    }
  }
}

struct page *pmm_alloc_order(u32 order)
{
  struct page *block;
  struct list_head *node = 0;
  struct page *page;
  
  if(!list_empty(&free_buddies[order].head))
  {
    page = (struct page *) free_buddies[order].head.next;
    list_del(&page->head); 
    goto out;
  }

  size_t i;
  for(i = max_order; order < i; i--)
  {
    if(list_empty(&free_buddies[i - 1].head) &&
      !list_empty(&free_buddies[i].head))
    {
      page = (struct page *) free_buddies[i].head.next;
      list_del(&page->head);
      list_add(&page->head, &free_buddies[i - 1].head);
      list_add((struct list_head *) get_buddy(page, i), &page->head);
    }
  }

out:
  return page;
}

void pmm_free_order(struct page *)
{

}
