#ifndef _INCLUDE_EXCEPTION_
#define _INCLUDE_EXCEPTION_

#include <ant/types.h>

#define NUMBER_OF_EXCEPTIONS 21

struct isr_context
{
  u64 r9;
  u64 r10;
  u64 r11;
  u64 r12;
  u64 r13;
  u64 r14;
  u64 r15;
  u64 rbp;
  u64 rbx;
  u64 rax;
  u64 rcx;
  u64 rdx;
  u64 rsi;
  u64 rdi;
  // This is the number of IRQ, Exception, etc...
  u64 isr_number;
  u64 rip;
  u64 cs;
  u64 rflags;
  u64 rsp;
  u64 ss;
} __attribute__ ((packed));

void isr_handler_c(struct isr_context *); 

#endif
