#include <arch/map.h>
#include <arch/syscall.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <fb/fb.h>
#include <acpi/acpi.h>
#include <mm/heap.h>
#include <arch/int.h>
#include <fs/vfs.h>
#include <libk/kprintf.h>
#include <kernel/exec.h>
#include <kernel/sched/sched.h>

void init(void)
{
  exec_execve("/userland/hello_world", NULL, NULL);
  int_timer_enable();
}

int kmain(void)
{
  pmm_init();
  vmm_init();
  fb_init();
  acpi_init();
  heap_init();
  kprintf("Stating VFS...\n");
  vfs_init();
  kprintf("Stating Sched...\n");
  sched_init();
  kprintf("Stating Interrupts...\n");
  int_init();
  kprintf("Stating Syscalls...\n\n");
  syscall_init();
  init();
 
  return 0;
}
