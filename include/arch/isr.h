#ifndef _INCLUDE_EXCEPTION_
#define _INCLUDE_EXCEPTION_

#include <ant/types.h>
#include <arch/x86_64/context.h>

#define NUMBER_OF_EXCEPTIONS 21

void isr_handler_c(struct context *); 

#endif
