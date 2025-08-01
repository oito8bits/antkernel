#include <arch/map.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <ant/boot.h>
extern u64 top_table_level;

int kmain(void)
{
  struct boot_info *boot_info = boot_get_info();
  struct pmm_area pmm_area;
  pmm_init(boot_info, &pmm_area);
  map_init(&pmm_area.table_area);
  vmm_init(boot_info, (struct table_entry *) &top_table_level);
  return 0;
}
