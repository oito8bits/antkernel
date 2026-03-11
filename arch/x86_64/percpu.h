#ifndef _INCLUDE_PERCPU_
#define _INCLUDE_PERCPU_

#include "tss.h"

struct percpu
{
  void *user_rsp;
  struct tss *tss;
};

void percpu_init(void);

#endif
