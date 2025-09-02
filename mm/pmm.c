#include <mm/pmm.h>

#include <mm/memmap.h>
#include <libk/string.h>
#include <libk/stddef.h>
#include <mm/early_heap.h>
#include <mm/bitset.h>
#include <arch/x86_64/pg.h>
#include <ant/align.h>

static u64 *pages;
static size_t npages;
static size_t nentries;

static size_t addr_to_idx(phys_addr_t addr)
{
  return addr / PAGE_SIZE;
}

static void alloc_reserved_pages(struct boot_info *boot_info)
{
  // Alloc the NULL addr. :)
  pmm_alloc_addr(0);

  struct memory_map *map = &boot_info->map;
  efi_memory_descriptor *descriptor = map->memory_descriptor;
  size_t i, j;
  for(i = 0; i < map->memory_map_size / map->descriptor_size; i++)
  {
    if(memmap_get_memory_type(descriptor->type) == available_memory)
      goto out;
     
    for(j = 0; j < descriptor->number_of_pages; j++)
      pmm_alloc_addr(descriptor->physical_start + j * PAGE_SIZE);

    out:
    descriptor = (void *) descriptor + map->descriptor_size;
  }

  struct gop_video_mode *mode = &boot_info->mode; 
  phys_addr_t base = (phys_addr_t) mode->frame_buffer_base;
  if(base + mode->frame_buffer_size > npages * PAGE_SIZE)
    return;

  for(i = 0; i < mode->frame_buffer_size; i += 4096)
    pmm_alloc_addr(base + i);
}

void pmm_init_area(struct area *area, phys_addr_t start, size_t area_npages)
{
  area->npages = area_npages;
  area->nentries = area_npages / 64;
  area->start = start;
  area->pages = pages + start / PAGE_SIZE / 64;
}

void *pmm_alloc_page(struct area *area)
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
        return (void *) (area->start + pos * PAGE_SIZE);
      }
    }
  }

  return NULL;
}

void *pmm_alloc_available_page(struct area *area)
{
  void *addr = NULL;
  size_t i;
  for(i = 0; i < AVAILABLE_AREAS; i++)
  {
    if((addr = pmm_alloc_page(area)) != NULL)
    {
      kprintf("addr: %i\n", addr);
      break;
    }
  }

  return addr;
}

s64 pmm_alloc_addr(phys_addr_t addr)
{
  size_t idx = addr_to_idx(addr);
  if(!bitset_is_set(pages, idx))
  {
    bitset_set(pages, idx);
    return idx;
  }

  return -1;
}

void pmm_free_page(struct area *area, void *addr)
{
  bitset_reset(area->pages, addr_to_idx((phys_addr_t) addr));
}

void pmm_init(struct boot_info *boot_info, struct pmm_area *parea)
{
  npages = memmap_get_memory_pages(&boot_info->map);
  nentries = npages / 64;
  pages = early_malloc(npages * 8);
  memset(pages, 0, nentries * 8);
  
  alloc_reserved_pages(boot_info);
  
  pmm_init_area(&parea->available_area[0],
                0, (boot_info->kernel_entry) / PAGE_SIZE);
  
  pmm_init_area(&parea->kernel_area, 
                boot_info->kernel_entry,
                boot_info->kernel_size / PAGE_SIZE);

  phys_addr_t table_area_pa = boot_info->kernel_entry + boot_info->kernel_size;
  if(IS_ALIGN(table_area_pa, PAGE_SIZE))
    table_area_pa = ALIGNUP(table_area_pa, PAGE_SIZE); 

  size_t table_area_size = 20 * (1 << 2);
  pmm_init_area(&parea->table_area,
                table_area_pa,
                table_area_size / PAGE_SIZE); /* 20 MiB */

  phys_addr_t available_area_pa = table_area_pa + table_area_size;
  if(IS_ALIGN(available_area_pa, PAGE_SIZE))
    table_area_pa = ALIGNUP(available_area_pa, PAGE_SIZE);
  
  pmm_init_area(&parea->available_area[1],
                available_area_pa,
                npages - available_area_pa / PAGE_SIZE);
}
