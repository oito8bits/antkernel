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
  
  return 0;
}
