#ifndef _INCLUDE_INT_
#define _INCLUDE_INT_

#define IRQ_BASE 32
#define IRQ_IOAPIC_BASE (IRQ_BASE + 1)

void int_timer_enable(void);
void int_init(void);

#endif
