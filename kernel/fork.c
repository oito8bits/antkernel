#include <kernel/fork.h>
#include <arch/timer.h>
#include <kernel/sched/sched.h>

int fork(struct context *ctx)
{
  struct sched_process *current_process = sched_get_current_process();
  struct sched_process *process = sched_create_process(current_process->path, READY);
  sched_add_process(sched_copy_process(process, ctx));
  
  return process->pid;
}
