#include <pg.h>
#include "cr.h"

u64 pg_get_l4_idx(virt_addr_t virt_addr)
{
  return virt_addr >> 39 & 0x1ff;
}

u64 pg_get_l3_idx(virt_addr_t virt_addr)
{
  return virt_addr >> 30 & 0x1ff; 
}

u64 pg_get_l2_idx(virt_addr_t virt_addr)
{
  return virt_addr >> 21 & 0x1ff;
}

u64 pg_get_l1_idx(virt_addr_t virt_addr)
{
  return virt_addr >> 12 & 0x1ff;
}

u64 pg_get_l0_idx(virt_addr_t virt_addr)
{
  return virt_addr & 0xfff;
}

phys_addr_t pg_virt_to_phys(virt_addr_t addr)
{
  return addr - VIRTUAL_BASE;
}

virt_addr_t pg_phys_to_virt(phys_addr_t addr)
{
  return VIRTUAL_BASE + addr;
}

phys_addr_t pg_get_table_entry_pa(struct table_entry *entry)
{
  return entry->phys_addr << 12;
}

void pg_switch_top_table(phys_addr_t top_table)
{
  cr_set_cr3((u64) top_table);
}

void pg_set_table_entry_pa(struct table_entry *entry, phys_addr_t addr)
{
  entry->phys_addr = addr >> 12;
}

void pg_set_page_entry_pa(struct page_entry *entry, phys_addr_t addr)
{
  entry->phys_addr = addr >> 12;
}

void pg_set_page_entry(struct page_entry *entry, phys_addr_t addr, u64 attr)
{
  entry->phys_addr = (u64) addr >> 12;
  u64 *entry_p = (u64 *) entry;
  *entry_p |=  attr;
}

void pg_set_table_entry(struct table_entry *entry, phys_addr_t addr, u64 attr)
{
  entry->phys_addr = (u64) addr >> 12;
  u64 *entry_p = (u64 *) entry;
  *entry_p |=  attr;
}
