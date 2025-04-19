#ifndef _INCLUDE_PAGING_
#define _INCLUDE_PAGING_

#include <ant/types.h>
#include <ant/boot.h>

#define PAGE_SIZE 4096

#define ALIGNUP(value, align) ((value) + ((align) - 1) & ~((align) - 1))

struct pde
{
  u32 p:1;
  u32 rw: 1;
  u32 us: 1;
  u32 pwt: 1;
  u32 pcd: 1;
  u32 a: 1;
  u32 ignored1: 1;
  u32 ps: 1;
  u32 ignored2: 3;
  u32 r: 1;
  u64 phys_addr: 40;
  u32 ignored3: 11;
  u32 xd: 1; 
};

struct pte
{
  u32 p:1;
  u32 rw: 1;
  u32 us: 1;
  u32 pwt: 1;
  u32 pcd: 1;
  u32 a: 1;
  u32 d: 1;
  u32 pat: 1;
  u32 g: 1;
  u32 ignored: 2;
  u32 r: 1;
  u64 phys_addr: 40; 
  u32 ignored3: 11;
  u32 xd: 1; 
};

#endif
