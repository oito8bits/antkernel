#include <kernel/exec.h>
#include <kernel/sched/sched.h>
#include <kernel/elf.h>
#include <libk/stddef.h>

int exec_execve(const char *path, const char *arg, const char *env)
{
  struct sched_process *process = sched_create_process(path, READY);
  if(elf_parse(&process->elf, path) < 0)
    return -1;

  elf_load(&process->elf, process->top_table);
  sched_create_thread(process, "idle thread", (void *) process->elf.header.entry, NULL, USER_SPACE);
  sched_add_process(process);

  sched_destroy_curr_process();
/*
  struct sched_process *current_process = sched_get_current_process();

  void *current_top_table = current_process->top_table;
  void *top_table = pg_phys_to_virt(pmm_alloc_table());
  vmm_kappend_kernel_space(top_table);  

  current_process->top_table = top_table;
  elf_parse(&current_process->elf, path);
  elf_load(&current_process->elf, top_table);
  sched_destroy_threads(&current_process->threads);
  pmm_free_table(pg_virt_to_phys(current_top_table));
  sched_create_thread(current_process, "idle thread", (void *) current_process->elf.header.entry, NULL, USER_SPACE);
*/
  return 0;
}
