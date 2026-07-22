#include <arch/isr.h>
#include <arch/int.h>
#include <arch/x86_64/lapic.h>
#include <arch/x86_64/ioapic.h>
#include <drivers/keyboard/ps2.h>
#include <kernel/sched/sched.h>
#include <libk/kprintf.h>

static void print_context_exception(struct context *ctx)
{
  kprintf("Exception: %#lx\n", ctx->isr_number);
  kprintf("RAX: %#lx, RBX: %#lx, RCX: %#lx, RDX: %#lx\n"
          "RSI: %#lx, RDI: %#lx, R8:  %#lx, R9:  %#lx\n"
          "R10: %#lx, R11: %#lx, R12: %#lx, R13: %#lx\n"
          "R14: %#lx, R15: %#lx, RBP: %#lx, RSP: %#lx\n"
          "RIP: %#lx, CS:  %#lx, SS:  %#lx, RFLAGS: %#lx\n",
          ctx->rax, ctx->rbx, ctx->rcx, ctx->rdx,
          ctx->rsi, ctx->rdi, ctx->r8, ctx->r9,
          ctx->r10, ctx->r11, ctx->r12, ctx->r13,
          ctx->r14, ctx->r15, ctx->rbp, ctx->rsp,
          ctx->rip, ctx->cs, ctx->ss, ctx->rdx,
          ctx->rax, ctx->rbx, ctx->rcx, ctx->rdx, ctx->rflags);
  while(1); 
}

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

  if(isr_number < 22)
    print_context_exception(ctx);

  if(isr_number >= IRQ_BASE)
    lapic_eoi();
}
