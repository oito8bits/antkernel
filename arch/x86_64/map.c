#include <arch/map.h>
#include <fb/fb.h>
#include <libk/string.h>
#include <libk/stddef.h>
#include <mm/pmm.h>

static size_t get_idx(void *virt_addr, size_t level)
{
  switch(level)
  {
    case 4:
      return pg_get_l4_idx(virt_addr);
    case 3:
      return pg_get_l3_idx(virt_addr);
    case 2:
      return pg_get_l2_idx(virt_addr);
  }
}

static struct table_entry *get_table_entry(struct table_entry *table, void *virt_addr, size_t level)
{
  return &table[get_idx(virt_addr, level)];
}

static struct table_entry *get_table_virt_addr(struct table_entry *table, void *virt_addr, size_t level)
{
  struct table_entry *entry = get_table_entry(table, virt_addr, level);
  phys_addr_t table_phys_addr = pg_get_table_entry_pa(entry);
  
  return (struct table_entry *) pg_phys_to_virt(table_phys_addr);
}

static struct table_entry *create_entry(struct table_entry *table, void *virt_addr, size_t level, u64 attr)
{
  struct table_entry *entry = get_table_entry(table, virt_addr, level);
  phys_addr_t table_pa = pg_get_table_entry_pa(entry);
  if(!entry->p)
  {
    table_pa = (phys_addr_t) pmm_alloc_table();
    memset((void *) pg_phys_to_virt(table_pa), 0, PAGE_SIZE);
    pg_set_table_entry(entry, table_pa, attr);
    table[get_idx(virt_addr, level)] = *entry;
  }
  
  return (struct table_entry *) pg_phys_to_virt(table_pa);
}

static void set_page_entry(struct page_entry *table, void *virt_addr, phys_addr_t phys_addr, u64 attr)
{
  size_t idx = pg_get_l1_idx(virt_addr);
  struct page_entry entry = table[idx];
  pg_set_page_entry(&entry, phys_addr, attr);
  table[idx] = entry;
}

void map(struct table_entry *table, phys_addr_t phys_addr, void *virt_addr, u64 attr)
{
  struct table_entry *l4, *l3, *l2;
  struct page_entry *l1;
  l4 = table;
  l3 = create_entry(l4, virt_addr, 4, attr);
  l2 = create_entry(l3, virt_addr, 3, attr);
  l1 = (struct page_entry *) create_entry(l2, virt_addr, 2, attr);
  set_page_entry(l1, virt_addr, phys_addr, attr);
}

void map_pages(struct table_entry *table, phys_addr_t phys_addr, void *virt_addr, u64 attr, u64 npages)
{
  size_t i;
  for(i = 0; i < npages; i++)
    map(table, phys_addr + i * PAGE_SIZE, virt_addr + i * PAGE_SIZE, attr);
}

bool is_unused_table(struct table_entry *table)
{
  size_t i, nentries = sizeof(struct page_entry);
  for(i = 0; i < nentries; i++)
  {
    if(table[i].p)
      return false;
  }

  return true;
}

void unmap(struct table_entry *table, void *virt_addr)
{
  struct table_entry *l4, *l3, *l2;
  struct page_entry *l1;

  l4 = table;
  l3 = get_table_virt_addr(l4, virt_addr, 4);
  l2 = get_table_virt_addr(l3, virt_addr, 3);
  l1 = (struct page_entry *) get_table_virt_addr(l2, virt_addr, 2);
  l1[pg_get_l1_idx(virt_addr)].p = 0;
  if(!is_unused_table((struct table_entry *) l1))
    return;

  size_t idx = get_idx(virt_addr, 2);
  l2[idx].p = 0;
  pmm_free_table(pg_virt_to_phys(l1));
  if(!is_unused_table(l2))
    return;
  
  idx = get_idx(virt_addr, 3);
  l3[idx].p = 0;
  pmm_free_table(pg_virt_to_phys(l2));
  if(!is_unused_table(l3))
    return;

  idx = get_idx(virt_addr, 4);
  l3[idx].p = 0;
  pmm_free_table(pg_virt_to_phys(l3));
}

void map_init(struct area *area)
{
}
