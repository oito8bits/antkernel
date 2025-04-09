#ifndef _INCLUDE_ZONES_
#define _INCLUDE_ZONES_

#include <ant/boot.h>
#include <ant/types.h>
#include <ant/list.h>

#define DMA_ZONE_SIZE (1 << 24)

#define DMA_ZONE_IDX 0
#define NORMAL_ZONE_IDX 1

#define AREA_ORDER 11
#define ZONES 2

struct area
{
  struct list_head list;
  u32 nareas;
};

struct zone
{
  u32 npages;
  struct area area[AREA_ORDER];
};

void zones_init(struct memory_map *);

#endif
