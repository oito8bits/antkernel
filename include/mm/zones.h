#ifndef _INCLUDE_ZONES_
#define _INCLUDE_ZONES_

#include <ant/boot.h>
#include <ant/types.h>
#include <ant/list.h>

#define AREA_ORDER 11

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
