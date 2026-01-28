#include <arch/x86_64/context.h>
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
