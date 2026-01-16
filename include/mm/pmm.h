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

enum pmm_area_type
{
  PMM_TABLE_AREA,
  PMM_BITMAP_AREA,
  PMM_RAMFS_AREA
};

void pmm_init_area(struct area *, phys_addr_t, size_t);
phys_addr_t pmm_alloc_page(struct area *);
phys_addr_t pmm_alloc_avail_page(void);
s64 pmm_alloc_addr(phys_addr_t);
void pmm_alloc_range(phys_addr_t addr, size_t pages);
phys_addr_t pmm_alloc_table(void);
void pmm_free_table(phys_addr_t);
void *pmm_get_area_addr(enum pmm_area_type);
void pmm_free_page(struct area *, phys_addr_t);
void pmm_init(void);

#endif
