#ifndef _INCLUDE_SCHED_
#define _INCLUDE_SCHED_

#include <ant/types.h>
#include <ant/list.h>
#include <arch/isr.h>

enum process_status
{
  READY,
  RUNNING,
  DEAD
};

struct sched_process
{
  struct list_head head;
  enum process_status status;
  struct isr_context *context;
};

void sched(void);
void sched_init(void);

#endif
