#ifndef _INCLUDE_PMM_
#define _INCLUDE_PMM_

#include <ant/list.h>
#include <ant/types.h>

#define AVAILABLE_AREAS 2

struct area
{
  phys_addr_t start;
  struct list_head head;
  u64 *pages;
  size_t npages;
  size_t nentries;
};

struct pmm_area
{
  struct area available_area[AVAILABLE_AREAS];
  struct area ramfs_area;
  struct area table_area;
  struct area bitmap_area;
};

void pmm_init_area(struct area *, phys_addr_t, size_t);
phys_addr_t pmm_alloc_page(struct area *);
phys_addr_t pmm_alloc_avail_page(void);
s64 pmm_alloc_addr(phys_addr_t);
void pmm_free_page(struct area *, phys_addr_t);
void pmm_init(struct pmm_area *);

#endif
