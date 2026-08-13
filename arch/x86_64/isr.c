#include <arch/isr.h>
#include <arch/int.h>
#include <arch/x86_64/lapic.h>
#include <arch/x86_64/ioapic.h>
#include <drivers/keyboard/ps2.h>
#include <kernel/sched/sched.h>
#include <stdio.h>

static void print_context_exception(struct context *ctx)
{
  printf("Exception: %#lx\n", ctx->isr_number);
  printf("RAX: %016lx, RBX: %016lx, RCX: %016lx, RDX: %016lx\n"
         "RSI: %016lx, RDI: %016lx, R8:  %016lx, R9:  %016lx\n"
         "R10: %016lx, R11: %016lx, R12: %016lx, R13: %016lx\n"
         "R14: %016lx, R15: %016lx, RBP: %016lx, RSP: %016lx\n"
         "RIP: %016lx, CS:  %016lx, SS:  %016lx, RFLAGS: %016lx\n",
         ctx->rax, ctx->rbx, ctx->rcx, ctx->rdx,
         ctx->rsi, ctx->rdi, ctx->r8,  ctx->r9,
         ctx->r10, ctx->r11, ctx->r12, ctx->r13,
         ctx->r14, ctx->r15, ctx->rbp, ctx->rsp,
         ctx->rip, ctx->cs,  ctx->ss,  ctx->rflags);
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
