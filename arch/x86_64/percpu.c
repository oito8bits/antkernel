#include "percpu.h"
#include "msr.h"

extern u64 _end_stack;
struct percpu cpu;

static void set_gs_base(void)
{
  wrmsr(0xC0000101, (u64) &cpu);
  wrmsr(0xC0000102, (u64) &cpu);
}

void percpu_init(void)
{
  cpu.tss = tss_get_base();
  set_gs_base();
}
