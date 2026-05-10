#include <kernel/fork.h>
#include <arch/timer.h>
#include <kernel/sched/sched.h>

int fork(void)
{
  timer_disable();
  
  struct sched_process *current_process = sched_get_current_process();
  if(current_process->status & FORK)
  {
    current_process->status &= ~FORK;
    return 0;
  }

  struct sched_process *process = sched_create_process(current_process->path, READY | FORK);
  sched_add_process(sched_copy_process(process, current_process));
  
  timer_enable();

  return process->pid;
}
