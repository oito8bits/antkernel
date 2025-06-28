#ifndef _INCLUDE_PMM_
#define _INCLUDE_PMM_

#include <ant/list.h>
#include <ant/boot.h>
#include <ant/types.h>

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
  struct area *available_area;
  struct area *kernel_area;
};

void pmm_init_area(struct area *, phys_addr_t, size_t);
void *pmm_alloc_area(struct area *);
void *pmm_alloc_page(struct area *);
void pmm_alloc_range(struct area *, phys_addr_t, size_t);
void pmm_free_page(struct area *, void *);
void pmm_init(struct boot_info *, struct pmm_area *);

#endif
