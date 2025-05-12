#ifndef _INCLUDE_PMM_
#define _INCLUDE_PMM_

#include <ant/boot.h>
#include <ant/types.h>
#include <ant/list.h>

#define MIN_ORDER 11

struct page
{
  struct list_head head;
};

void pmm_init(struct boot_info *);
struct page *pmm_alloc_order(u32);
void show_buddies(void);

#endif
