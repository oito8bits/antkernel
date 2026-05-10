#include <arch/x86_64/lapic.h>

void timer_enable(void)
{
  lapic_write_reg(LAPIC_LVT_TMR, lapic_read_reg(LAPIC_LVT_TMR) & ~(1 << 16));
}

void timer_disable(void)
{
  lapic_write_reg(LAPIC_LVT_TMR, lapic_read_reg(LAPIC_LVT_TMR) | (1 << 16));
}
