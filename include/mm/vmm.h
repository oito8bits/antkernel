#ifndef _INCLUDE_VMM_
#define _INCLUDE_VMM_

#include <ant/boot.h>
#include <pg.h>
#include <mm/pmm.h>

void vmm_init(struct boot_info *, struct pmm_area *);

#endif
