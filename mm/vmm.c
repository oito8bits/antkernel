#include <mm/vmm.h>
#include <arch/map.h>
#include <pg.h>
#include <ant/align.h>

virt_addr_t _start_text;
virt_addr_t _end_text;

virt_addr_t _start_data;
virt_addr_t _end_data;

virt_addr_t _start_rodata;
virt_addr_t _end_rodata;

virt_addr_t _start_stack;
virt_addr_t _end_stack;

virt_addr_t _start_bss;
virt_addr_t _end_bss;

virt_addr_t _start_brk;
virt_addr_t _end_brk;

extern struct table_entry kernel_top_table;

static void map_section(void *start_addr,
                        void *end_addr,
                        u64 attr)
{
  u64 size = end_addr - start_addr;

  if(IS_ALIGN(size, PAGE_SIZE))
    size = ALIGNUP(size, PAGE_SIZE);

  map_pages(&kernel_top_table, 
            (phys_addr_t) pg_virt_to_phys((virt_addr_t) start_addr),
            (virt_addr_t )start_addr,
            attr,
            size / PAGE_SIZE);
}

static void map_kernel(void)
{
  map_section(&_start_text, &_end_text, BIT_PRESENT);
  map_section(&_start_data, &_end_data, BIT_PRESENT | BIT_WRITE);
  map_section(&_start_rodata, &_end_rodata, BIT_PRESENT);
  map_section(&_start_stack, &_end_stack, BIT_PRESENT | BIT_WRITE);
  map_section(&_start_bss, &_end_bss, BIT_PRESENT | BIT_WRITE);
  map_section(&_start_brk, &_end_brk, BIT_PRESENT | BIT_WRITE);

  pg_switch_top_table(pg_virt_to_phys((virt_addr_t) &kernel_top_table));
}

static void free_available_pages(struct memory_map *map)
{
/*
  memset(pages, 0, nentries * 8);

  efi_memory_descriptor *descriptor = map->memory_descriptor;

  size_t i;
  for(i = 0; i < map->memory_map_size / map->descriptor_size; i++)
  {

  }
*/
}

void vmm_get_pages(size_t npages)
{
}

void vmm_init(struct boot_info *boot_info, struct pmm_area *pmm_area)
{
  phys_addr_t table_area = pmm_area->table_area.start;
  map_pages(&kernel_top_table, table_area, pg_phys_to_virt(table_area), BIT_PRESENT | BIT_WRITE, pmm_area->table_area.npages);
  map_kernel();
}
