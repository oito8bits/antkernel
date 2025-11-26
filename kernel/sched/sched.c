#include <kernel/sched/sched.h>

struct sched_process process;

void sched(void)
{
}

void sched_init(void)
{
  list_head_init(&process.head);
}
