#include <arch/map.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <fb/fb.h>
#include <acpi/acpi.h>
#include <mm/heap.h>
#include <arch/int.h>
#include <fs/vfs.h>
#include <kernel/exec.h>
#include <kernel/sched/sched.h>

int kmain(void)
{
  struct pmm_area pmm_area;
  
  pmm_init(&pmm_area);
  map_init(&pmm_area.table_area);
  vmm_init(&pmm_area);
  fb_init();
  acpi_init();
  heap_init();
  vfs_init();
  sched_init();
  int_init();

// Just testing....
#include <libk/kprintf.h>
  vfs_mount("", "/dev/", "devfs");
  vfs_mount("/dev/ramdisk", "/", "ramfs");
  //int fd = exec_execve("/userland/shell", NULL, NULL);
  //if(fd < 0)
    kprintf("Failed to open file.\n");
  kprintf("I'm still alive."); 
  return 0;
}
