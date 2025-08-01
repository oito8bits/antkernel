#ifndef _INCLUDE_TABLE_ENTRY_
#define _INCLUDE_TABLE_ENTRY_

#include <ant/types.h>
#include <ant/boot.h>

#define PAGE_SIZE 4096

#define BIT_PRESENT       (1ULL << 0)
#define BIT_WRITE         (1ULL << 1)
#define BIT_USER          (1ULL << 2)
#define BIT_WRITE_THROUGH (1ULL << 3) 
#define BIT_CACHE_DISABLE (1ULL << 4)
#define BIT_ACCESSED      (1ULL << 5)
#define BIT_PAGE_SIZE     (1ULL << 7)

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
phys_addr_t pg_virt_to_phys(virt_addr_t addr);
virt_addr_t pg_phys_to_virt(phys_addr_t addr);
phys_addr_t pg_get_table_entry_pa(struct table_entry *);
void pg_set_table_entry_pa(struct table_entry *entry, phys_addr_t addr);
void pg_set_page_entry_pa(struct page_entry *entry, phys_addr_t addr);
void pg_set_page_entry(struct page_entry *entry, phys_addr_t addr, u64 attr);
void pg_set_table_entry(struct table_entry *entry, phys_addr_t addr, u64 attr);

#endif
