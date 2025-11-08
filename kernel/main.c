#include <arch/map.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <fb/fb.h>
#include <acpi/acpi.h>
#include <mm/heap.h>
#include <arch/int.h>
#include <fs/vfs.h>

int kmain(void)
{
  struct pmm_area pmm_area;
  
  pmm_init(&pmm_area);
  map_init(&pmm_area.table_area);
  vmm_init(&pmm_area);
  fb_init();
  acpi_init();
  int_init();
  heap_init();
  vfs_init();

  return 0;
}
