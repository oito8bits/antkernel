#ifndef _INCLUDE_TABLE_ENTRY_
#define _INCLUDE_TABLE_ENTRY_

#include <ant/types.h>
#include <ant/boot.h>

#define PAGE_SIZE 4096

struct table_entry
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

struct page_entry
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

u64 pg_get_l4_idx(virt_addr_t virt_addr);
u64 pg_get_l3_idx(virt_addr_t virt_addr);
u64 pg_get_l2_idx(virt_addr_t virt_addr);
u64 pg_get_l1_idx(virt_addr_t virt_addr);

#endif
