#include <mm/pmm.h>
#include <mm/map.h>
#include <table_entry.h>
#include <libk/kprintf.h>

phys_addr_t start_addr;
phys_addr_t end_addr;

void pmm_init(struct boot_info *boot_info)
{
  u64 mem_pages = map_get_memory_pages(&boot_info->map);
  start_addr = boot_info->kernel_entry + boot_info->kernel_size;
  end_addr = start_addr + mem_pages * PAGE_SIZE;
}
