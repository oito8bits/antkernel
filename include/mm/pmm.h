#ifndef _INCLUDE_PMM_
#define _INCLUDE_PMM_

#include <ant/boot.h>
#include <ant/types.h>
#include <ant/list.h>

#define MIN_ORDER 11

struct page
{
  struct list_head head;
  u32 order;
};

void pmm_init(struct boot_info *);
struct page *pmm_alloc_order(u32);
void pmm_free_order(struct page *);
void show_buddies(void);

#endif
