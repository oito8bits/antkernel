#include <arch/isr.h>
#include <arch/int.h>
#include <arch/x86_64/lapic.h>
#include <arch/x86_64/ioapic.h>
#include <drivers/keyboard/ps2.h>
#include <kernel/sched/sched.h>

void isr_handler_c(struct context *ctx)
{
  int isr_number = ctx->isr_number;
  
  switch(isr_number)
  {
    case 32:
      lapic_timer_isr();
      sched(ctx);
      break;
    case 34:
      keyboard_irq_handler();
      break;
  }

  if(isr_number >= IRQ_BASE)
    lapic_eoi();
}
