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

static void exec_shell(const char *path, const char *arg, const char *env)
{
  struct sched_process *process = sched_create_process(path, READY);
  elf_parse(&process->elf, path);
  elf_load(&process->elf, process->top_table);
  sched_create_thread(process, "idle thread", (void *) process->elf.header.entry, NULL, USER_SPACE);
  sched_add_process(process);
}

static void init(void)
{
  exec_shell("/userland/shell", NULL, NULL);
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
