#include <arch/map.h>
#include <fb/fb.h>
#include <libk/string.h>
#include <libk/stddef.h>
#include <mm/pmm.h>

struct area *table_area;

static void *get_table_entry(struct table_entry *table, u64 virt_addr, u64 attr, size_t level)
{
  size_t idx;
  switch(level)
  {
    case 4:
      idx = pg_get_l4_idx(virt_addr);
      break;
    case 3:
      idx = pg_get_l3_idx(virt_addr);
      break;
    case 2:
      idx = pg_get_l2_idx(virt_addr);
      break;
  }
 
  struct table_entry entry = table[idx];
  phys_addr_t table_pa = pg_get_table_entry_pa(&entry);
  if(!entry.p)
  {
    table_pa = (phys_addr_t) pmm_alloc_page(table_area);
    memset((void *) pg_phys_to_virt(table_pa), 0, PAGE_SIZE);
    pg_set_table_entry(&entry, table_pa, attr);
    table[idx] = entry;
  }

  return (struct table_entry *) pg_phys_to_virt(table_pa);
}

static void set_page_entry(struct page_entry *table, virt_addr_t virt_addr, phys_addr_t phys_addr, u64 attr)
{
  size_t idx = pg_get_l1_idx(virt_addr);
  struct page_entry entry = table[idx];
  pg_set_page_entry(&entry, phys_addr, attr);
  table[idx] = entry;
}

void map(struct table_entry *table, phys_addr_t phys_addr, virt_addr_t virt_addr, u64 attr)
{
  struct table_entry *l4, *l3, *l2;
  struct page_entry *l1;

  l4 = table;
  l3 = get_table_entry(l4, virt_addr, attr, 4);
  l2 = get_table_entry(l3, virt_addr, attr, 3);
  l1 = get_table_entry(l2, virt_addr, attr, 2);
  set_page_entry((struct page_entry *) l1, virt_addr, phys_addr, attr);
}

void map_init(struct area *area)
{
  table_area = area;
}
