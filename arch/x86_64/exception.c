#include "exception.h"
#include <ant/types.h>
#include <libk/kprintf.h>

void exception_handler_c(unsigned long error_code)
{
  kprintf("Handling exception %i\n", error_code);
}
