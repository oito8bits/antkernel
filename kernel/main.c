#include <arch/map.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <ant/boot.h>
#include <fb/fb.h>
#include <acpi/acpi.h>
#include <mm/heap.h>
#include <arch/int.h>
#include <fs/vfs.h>

int kmain(void)
{
  struct boot_info *boot_info = boot_get_info();
  struct pmm_area pmm_area;
  
  pmm_init(boot_info, &pmm_area);
  map_init(&pmm_area.table_area);
  vmm_init(boot_info, &pmm_area);
  fb_init(boot_info);
  acpi_init(boot_info);
  int_init();
  vfs_init();
  heap_init();
  
  return 0;
}
