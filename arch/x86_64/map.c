#include <arch/map.h>
#include <libk/kprintf.h>
#include <libk/stddef.h>
#include <mm/early_heap.h>

// TODO

struct table_entry *get_table_entry(struct table_entry *table, u64 virt_addr, u64 attr, size_t level)
{
  struct table_entry *entry;
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
}

void map(struct table_entry *table, u64 phys_addr, u64 virt_addr, u64 attr)
{
/*
  struct table_entry *l4, *l3, *l2;
  struct table_entry *l1;

  l4 = table;
  l3 = get_table_entry(l4, virt_addr, attr, 4);
  l2 = get_table_entry(l3, virt_addr, attr, 3);
  l1 = get_table_entry(l2, virt_addr, attr, 2);
*/
}
