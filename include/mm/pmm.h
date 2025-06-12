#ifndef _INCLUDE_PMM_
#define _INCLUDE_PMM_

#include <ant/boot.h>
#include <ant/types.h>

s64 pmm_alloc_page(void);
void pmm_free_page(s64 page);
void pmm_init(struct boot_info *);

#endif
