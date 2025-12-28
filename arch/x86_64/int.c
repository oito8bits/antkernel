#include <arch/int.h>
#include <lapic.h>
#include <ioapic.h>

void int_timer_enable(void)
{
  lapic_timer_enable();
}

void int_init(void)
{
  lapic_init();
  ioapic_init();
}
