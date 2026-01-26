#include <arch/x86_64/context.h>
#include "tss.h"

void context_copy(struct context *dst, struct context *src)
{
  dst->rip = src->rip; 
  dst->rsp = src->rsp;
  dst->cs = src->cs;
  dst->ss = src->ss;
  /*
  new->r9 = old->r9; 
  new->r10 = old->r10; 
  new->r11 = old->r11; 
  new->r12 = old->r12; 
  new->r13 = old->r13; 
  new->r14 = old->r14; 
  new->r15 = old->r15; 
  new->rbp = old->rbp; 
  new->rbx = old->rbx; 
  new->rax = old->rax; 
  new->rcx = old->rcx; 
  new->rdx = old->rdx; 
  new->rsi = old->rsi; 
  new->rdi = old->rdi; 
  new->rflags = old->rflags; 
  */
}

void context_set_kstack(void *kstack_top)
{
  struct tss *tss = tss_get_base();
  
  tss->rsp0 = kstack_top;
}
