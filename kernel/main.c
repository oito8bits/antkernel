#include <arch/map.h>
#include <arch/syscall.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <fb/fb.h>
#include <acpi/acpi.h>
#include <mm/heap.h>
#include <arch/int.h>
#include <fs/vfs.h>
#include <kernel/exec.h>
#include <kernel/sched/sched.h>

void init(void)
{
  exec_execve("/userland/shell", NULL, NULL);
  int_timer_enable();
}

int kmain(void)
{
  pmm_init();
  vmm_init();
  fb_init();
  acpi_init();
  heap_init();
  vfs_init();
  sched_init();
  int_init();
  syscall_init();
  init();
 
  return 0;
}
