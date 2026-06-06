#ifndef _INCLUDE_PERCPU_
#define _INCLUDE_PERCPU_

#include <arch/x86_64/tss.h>

struct percpu
{
  void *user_rsp;
  struct tss *tss;
};

struct percpu *percpu_get_gs_base(void);
void percpu_init(void);

#endif
