#include <arch/x86_64/context.h>
#include <arch/percpu.h>
#include <libk/string.h>
#include "tss.h"

void context_copy(struct context *dst, struct context *src)
{
  memcpy(dst, src, sizeof(struct context));
}

void context_set_kstack(void *kstack_top)
{
  struct tss *tss = tss_get_base();
  
  tss->rsp0 = (u64) kstack_top;
}

void context_fork(struct context *dst, struct context *src)
{
  memcpy(dst, src, sizeof(struct context));

  dst->rax = 0;
  dst->rip = dst->rcx;
  dst->cs = 0x18 | 3;
  dst->ss = 0x20 | 3;
  dst->rflags = 0x202;

  struct percpu *cpu = percpu_get_gs_base();
  dst->rsp = (u64) cpu->user_rsp;
  dst->kernel_gs_base = (u64) cpu;
}
