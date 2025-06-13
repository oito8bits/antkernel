#include <mm/pmm.h>

#include <mm/memmap.h>
#include <libk/string.h>
#include <libk/stddef.h>
#include <mm/early_heap.h>
#include <mm/bitset.h>

u64 *pages;
u32 nentries;

s64 pmm_alloc_page(void)
{
  size_t i, j;
  for(i = 0; i < nentries; i++)
  {
    if(pages[i] == 0xffffffffffffffff)
      continue;
    
    for(j = 0; j < 64; j++)
    {
      size_t pos = i * 64 + j;
      if(!bitset_is_set(pages, pos))
      {
        bitset_set(pages, pos);
        return pos;
      }
    }
  }
  
  return -1;
}

s64 pmm_alloc_idx(size_t idx)
{
  if(!bitset_is_set(pages, idx))
  {
    bitset_set(pages, idx);
    return idx; 
  }

  return -1;
}

void pmm_free_page(s64 idx)
{
  bitset_reset(pages, idx);
}

void pmm_init(struct boot_info *boot_info)
{
  size_t npages = memmap_get_memory_pages(&boot_info->map);
  
  nentries = npages / 64;
  pages = early_malloc(nentries * 8);
  memset(pages, 0, nentries * 8);
}
