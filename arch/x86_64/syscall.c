#include <arch/syscall.h>
#include <arch/x86_64/context.h>
#include <fs/vfs.h>

#include <kernel/exec.h>
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

u64 syscall_handler(struct context *ctx)
{
  u64 ret;

  switch(ctx->rax)
  {
    case 0:
      ret = vfs_read(ctx->rdi, (void *) ctx->rsi, ctx->rdx);
      break;
    case 1:
      ret = vfs_write(ctx->rdi, (void *) ctx->rsi, ctx->rdx);
      break;
    case 2:
      ret = vfs_open((const char *) ctx->rdi, ctx->rsi);
      break;
    case 6:
      ret = vfs_close(ctx->rdi);
      break;
    case 8:
      ret = vfs_lseek(ctx->rdi, ctx->rsi, ctx->rdx);
      break;
    case 59:
      ret = exec_execve((const char *) ctx->rdi, (const char *) ctx->rsi, (const char *) ctx->rdx);
      break;
    case 60:
      exit(ctx->rdi);
      break;
  }

  return ret;
}
