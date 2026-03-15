#include <arch/syscall.h>
#include <arch/x86_64/context.h>
#include <fs/vfs.h>

#include <kernel/exit.h>

#include "msr.h"

#define IA32_STAR_MSR 0xC0000081
#define IA32_LSTAR_MSR 0xC0000082
#define IA32_EFER_MSR 0xC0000080

extern u64 entry;
void syscall_init()
{
  // Set CS and SS.
  wrmsr(IA32_STAR_MSR, (0x00180008UL << 32) | rdmsr(IA32_STAR_MSR));
  // Set sycall entrypoint.
  wrmsr(IA32_LSTAR_MSR, (u64) &entry);
  // Enable syscall and sysret
  wrmsr(IA32_EFER_MSR, rdmsr(IA32_EFER_MSR) | (1 << 0));
}

void syscall_handler(struct context *ctx)
{
  switch(ctx->rax)
  {
    case 0:
      vfs_read(ctx->rdi, (void *) ctx->rsi, ctx->rdx);
      break;
    case 1:
      vfs_write(ctx->rdi, (void *) ctx->rsi, ctx->rdx);
      break;
    case 60:
      exit(ctx->rdi);
      break;
  }
}
