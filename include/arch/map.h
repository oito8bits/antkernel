#ifndef _INCLUDE_MAP_
#define _INCLUDE_MAP_

#include <ant/types.h>
#include <mm/pmm.h>
#include <pg.h>

void map(struct table_entry *, phys_addr_t, void *, u64);
void map_pages(struct table_entry *, phys_addr_t, void *, u64, u64);
void unmap(struct table_entry *, void *);

#endif
