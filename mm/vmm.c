#include <mm/vmm.h>
#include <arch/map.h>
#include <pg.h>
#include <ant/align.h>
#include <mm/pmm.h>

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

  map_pages(kernel_top_table, 
            pg_virt_to_elf(start_addr),
            start_addr,
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

  pg_switch_top_table(pg_virt_to_elf(&kernel_top_table));
}

void vmm_map(struct table_entry *top_table, void *virt_addr, size_t npages, u64 attr)
{
  size_t i;
  for(i = 0; i < npages; i++)
    map(top_table, 
        pmm_alloc_avail_page(), 
        virt_addr + i * PAGE_SIZE, 
        attr);
}

void vmm_kmap(phys_addr_t phys_addr, void *virt_addr, u64 attr, size_t npages)
{
  map_pages((void *) kernel_top_table,
            phys_addr,
            virt_addr,
            attr,
            npages);
}

void vmm_kmap_pdata(phys_addr_t phys_addr, void *virt_addr, size_t npages)
{
  map_pages((void *) kernel_top_table,
            phys_addr,
            virt_addr,
            BIT_PRESENT | BIT_WRITE,
            npages);
}

void vmm_unmap(struct table_entry *top_table, void *virt_addr)
{

}

void vmm_init(struct boot_info *boot_info, struct pmm_area *pmm_area)
{
  phys_addr_t table_area = pmm_area->table_area.start;
  phys_addr_t bitmap_area = pmm_area->bitmap_area.start;
  phys_addr_t ramfs_area = pmm_area->ramfs_area.start;
  
  map_pages(kernel_top_table,
            table_area,
            pg_phys_to_virt(table_area),
            BIT_PRESENT | BIT_WRITE,
            pmm_area->table_area.npages);

  map_pages(kernel_top_table,
            bitmap_area,
            pg_phys_to_virt(bitmap_area),
            BIT_PRESENT | BIT_WRITE, 
            pmm_area->bitmap_area.npages);

  map_pages(kernel_top_table,
            ramfs_area,
            pg_phys_to_virt(ramfs_area),
            BIT_PRESENT | BIT_WRITE,
            pmm_area->ramfs_area.npages);

  map_kernel();
}
