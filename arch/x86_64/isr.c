#include "isr.h"
#include <ant/types.h>
#include <libk/kprintf.h>
#include <arch/x86_64/lapic.h>

void isr_handler_c(struct isr_context *ctx)
{
  kprintf("Handling exception: %lu\n", ctx->isr_number);
  if(ctx->isr_number == 32)
    lapic_eoi();
}
