#ifndef _INCLUDE_MAP_
#define _INCLUDE_MAP_

#include <ant/types.h>
#include <mm/pmm.h>
#include <pg.h>

void map(struct table_entry *, u64, u64, u64);
void map_init(struct area *area);

#endif
