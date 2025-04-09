#include <mm/zones.h>
#include <libk/kprintf.h>
#include <mm/map.h>

struct zone zones[ZONES];

void zones_init(struct memory_map *map)
{
  u64 pages = map_get_memory_pages(map);

  zones[DMA_ZONE_IDX].npages = DMA_ZONE_SIZE;
  zones[NORMAL_ZONE_IDX].npages = DMA_ZONE_SIZE - pages;
}
