#ifndef _INCLUDE_CR_
#define _INCLUDE_CR_

#include <ant/types.h>

static inline void cr_set_cr3(u64 top_table)
{
  __asm__ volatile("mov    %0, %%cr3" : : "r"(top_table));
}

static inline u64 cr_get_cr4(void)
{
  u64 cr4;
  __asm__ volatile("movq  %%cr4, %0": "=r" (cr4));
}

#endif
