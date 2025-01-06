#include <mm/zones.h>
#include <libk/kprintf.h>

void zones_init(struct memory_map *map)
{
  size_t i;
  for(i = 0; i < 10; i++)
    kprintf("number of pages: %#lx\n", map->memory_descriptor[i].number_of_pages);
}
