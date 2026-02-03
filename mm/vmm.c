#include <mm/vmm.h>
#include <arch/map.h>
#include <pg.h>
#include <ant/align.h>
#include <mm/pmm.h>
#include <ant/boot.h>
#include <libk/string.h>

u8 _start_text;
u8 _end_text;

u8 _start_data;
u8 _end_data;

u8 _start_rodata;
u8 _end_rodata;

u8 _start_stack;
u8 _end_stack;

u8 _start_bss;
u8 _end_bss;

u8 _start_brk;
u8 _end_brk;

struct table_entry kernel_top_table[512] __attribute__((aligned(4096)));

static void map_section(void *start_addr,
                        void *end_addr,
                        u64 attr)
{
  u64 size = end_addr - start_addr;

  if(IS_ALIGN(size, PAGE_SIZE))
    size = ALIGNUP(size, PAGE_SIZE);

  vmm_map(0, 
          pg_virt_to_elf(start_addr),
          start_addr,
          size / PAGE_SIZE,
          attr);
}

static void map_kernel(void)
{
  map_section(&_start_text, &_end_text, KERNEL_DATA);
  map_section(&_start_data, &_end_data, KERNEL_DATA);
  map_section(&_start_rodata, &_end_rodata, KERNEL_DATA);
  map_section(&_start_stack, &_end_stack, KERNEL_DATA);
  map_section(&_start_bss, &_end_bss, KERNEL_DATA);
  map_section(&_start_brk, &_end_brk, KERNEL_DATA);

  pg_switch_top_table(pg_virt_to_elf(&kernel_top_table));
}

void vmm_map(struct table_entry *top_table,
             phys_addr_t phys_addr,
             void *virt_addr, 
             size_t npages,
             u64 attr)
{
  if(!top_table)
    top_table = kernel_top_table;

  phys_addr_t paddr;
  size_t i;
  for(i = 0; i < npages; i++)
  {
    if(!phys_addr)
      paddr = pmm_alloc_avail_page();
    else
      paddr = phys_addr + i * PAGE_SIZE;

    map(top_table,
        paddr,
        virt_addr + i * PAGE_SIZE, 
        attr);
  }
}

void vmm_kappend_process_space(struct table_entry *top_table)
{
  memcpy(kernel_top_table, top_table, sizeof(struct table_entry) * 256);
}

void vmm_kappend_kernel_space(struct table_entry *top_table)
{
  memcpy(&top_table[256], &kernel_top_table[256], sizeof(struct table_entry) * 256);
}

void vmm_unmap(struct table_entry *top_table, void *virt_addr, size_t npages)
{
  size_t i;
  for(i = 0; i < npages; i++)
    unmap(top_table, virt_addr + i * PAGE_SIZE);
}

void vmm_init(void)
{
  struct boot_info *boot_info = boot_get_info();
  
  struct area *area;
 
  area = pmm_get_area_addr(PMM_TABLE_AREA);
  vmm_map(0,
          area->start,
          pg_phys_to_virt(area->start),
          area->npages,
          KERNEL_DATA);

  area = pmm_get_area_addr(PMM_BITMAP_AREA);
  vmm_map(0,
          area->start,
          pg_phys_to_virt(area->start),
          area->npages,
          KERNEL_DATA);

  area = pmm_get_area_addr(PMM_RAMFS_AREA);
  vmm_map(0,
          area->start,
          pg_phys_to_virt(area->start),
          area->npages,
          KERNEL_DATA);

  map_kernel();
}
