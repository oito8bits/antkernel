#include <mm/memmap.h>

size_t memmap_get_memory_pages(struct memory_map *map)
{
  efi_memory_descriptor *descriptor = map->memory_descriptor;
  size_t i;
  size_t pages = 0;
  for(i = 0; i < map->memory_map_size / map->descriptor_size; i++)
  {
    descriptor = (void *) descriptor + map->descriptor_size;
    pages += descriptor->number_of_pages;
  }

  return pages;
}
