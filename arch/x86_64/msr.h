#ifndef _INCLUDE_MSR_
#define _INCLUDE_MSR_

#include <ant/types.h>

#define IA32_APIC_BASE 0x1b

static inline u64 rdmsr(u32 msr)
{
  u64 msr_lo, msr_hi;
  __asm__ volatile("rdmsr" : "=a" (msr_lo), "=d" (msr_hi): "c"(msr));

  return msr_hi << 32 | msr_lo;
}

#endif
