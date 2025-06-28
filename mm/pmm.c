#include <mm/pmm.h>

#include <mm/memmap.h>
#include <libk/string.h>
#include <libk/stddef.h>
#include <mm/early_heap.h>
#include <mm/bitset.h>

static size_t addr_to_idx(struct area *area, phys_addr_t addr)
{
  return (addr - area->start) / 4096;
}

void pmm_init_area(struct area *area, phys_addr_t start, size_t npages)
{
  area->npages = npages;
  area->nentries = npages / 64;
  area->start = start;
  area->pages = early_malloc(npages * 8);
  memset(area->pages, 0, area->nentries * 8);
}

void *pmm_alloc_area(struct area *area)
{
  size_t i, j;
  for(i = 0; i < area->nentries; i++)
  {
    if(area->pages[i] == 0xffffffffffffffff)
      continue;

    for(j = 0; j < 64; j++)
    {
      size_t pos = i * 64 + j;
      if(!bitset_is_set(area->pages, pos))
      {
        bitset_set(area->pages, pos);
        return (void *) area->start + pos * 4096;
      }
    }
  }

  return NULL;
}

void *pmm_alloc_page(struct area *area)
{
  return pmm_alloc_area(area);
}

size_t alloc_addr(struct area *area, phys_addr_t addr)
{
  size_t idx = addr_to_idx(area, addr);
  if(!bitset_is_set(area->pages, idx))
  {
    bitset_set(area->pages, idx);
    return idx;
  }

  return -1;
}

void pmm_alloc_range(struct area *area, phys_addr_t addr, size_t npages)
{
  size_t i, idx;
  for(i = 0; i < npages; i++)
    alloc_addr(area, addr + i * 4096);
}

void pmm_free_page(struct area *area, void *addr)
{
  bitset_reset(area->pages, addr_to_idx(area, (phys_addr_t) addr));
}

void pmm_init(struct boot_info *boot_info, struct pmm_area *parea)
{
  //size_t npages = memmap_get_memory_pages(&boot_info->map);
  parea->kernel_area = early_malloc(sizeof(struct area));
  pmm_init_area(parea->kernel_area, 
                boot_info->kernel_entry,
                boot_info->kernel_size / 4096);
}
