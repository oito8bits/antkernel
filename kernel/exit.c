#include <kernel/exit.h> 
#include <kernel/sched/sched.h>

void exit(int error_code)
{
  sched_destroy_curr_process(); 
}
