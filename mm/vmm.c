#include <mm/vmm.h>
#include <arch/map.h>

static void free_available_pages(struct memory_map *map)
{
/*
  memset(pages, 0, nentries * 8);

  efi_memory_descriptor *descriptor = map->memory_descriptor;

  size_t i;
  for(i = 0; i < map->memory_map_size / map->descriptor_size; i++)
  {

  }
*/
}

void vmm_get_pages(size_t npages)
{
}

void vmm_init(struct boot_info *boot_info, struct table_entry *table)
{
  
}
