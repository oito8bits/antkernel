#include <arch/map.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <ant/boot.h>
#include <fb/fb.h>

extern u64 early_top_table;
struct table_entry kernel_top_table[512] __attribute__((aligned(4096)));

int kmain(void)
{
  struct boot_info *boot_info = boot_get_info();
  struct pmm_area pmm_area;
  size_t i;
  pmm_init(boot_info, &pmm_area);
  map_init(&pmm_area.table_area);
  vmm_init(boot_info, &pmm_area);
  fb_init(boot_info);
  return 0;
}
