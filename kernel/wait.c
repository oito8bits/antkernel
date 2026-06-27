#include <kernel/wait.h>
#include <kernel/sched/sched.h>
#include <arch/timer.h>

int wait_wait4(int pid,
               int *wstatus,
               int options,
               struct rusage *rusage,
               struct context *ctx)
{
  struct sched_thread *current_thread = sched_get_current_thread();
  if(current_thread == 0)
    return -1;

  timer_disable();
  context_fork(&current_thread->context, ctx);
  sched_wait(current_thread);
  timer_enable();

  sched_idle();

  return 0;
}
