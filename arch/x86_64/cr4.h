#ifndef _INCLUDE_CR4_
#define _INCLUDE_CR4_

#include <ant/types.h>

#define CR4_LA57 12

static inline u64 cr4_get(void)
{
  u64 cr4;
  __asm__ volatile("movq  %%cr4, %0": "=r" (cr4));
}

#endif
