#ifndef _INCLUDE_VMM_
#define _INCLUDE_VMM_

#include <ant/boot.h>
#include <pg.h>
#include <mm/pmm.h>

enum vmm_area_type
{
  low_area,
  high_area
};

struct varea
{
  void *start;
  size_t npages;
};

void vmm_map(struct table_entry *, void *, size_t, u64);
void vmm_kmap(phys_addr_t phys_addr, void *virt_addr, u64 attr, size_t npages);
void vmm_kmap_pdata(phys_addr_t, void *, size_t);
void vmm_init(struct boot_info *, struct pmm_area *);

#endif
