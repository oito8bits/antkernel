#include "isr.h"
#include <ant/types.h>
#include <libk/kprintf.h>
#include <arch/x86_64/lapic.h>
#include <arch/x86_64/ioapic.h>
#include <drivers/keyboard/ps2.h>

void isr_handler_c(struct isr_context *ctx)
{
  int isr_number = ctx->isr_number;
/*
  if(isr_number == 32)
  {
    lapic_timer_isr();
    lapic_eoi();
    return;
  } 
  else if(isr_number > 32)
  {
    kprintf("Keyboard is working...\n");
    lapic_eoi();
    return;
  }
  else if(isr_number <= 31)
  {
    kprintf("Handling exception. %i\n", isr_number);
  }
*/
  if(isr_number >= 32)
  {
    if(isr_number == 34)
      keyboard_irq_handler();
      
    lapic_eoi();
  }
}
