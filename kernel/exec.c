#include <kernel/exec.h>
#include <kernel/sched/sched.h>
#include <kernel/elf.h>
#include <fs/vfs.h>
#include <mm/heap.h>

int exec_execve(const char *path, const char *arg, const char *env)
{
  int fd = vfs_open(path, 0);

  if(fd < 0)
    return -1;

  struct sched_process *process = sched_create_process("idle process");
  process->elf.fd = fd;
  elf_parse(&process->elf);
  elf_load(&process->elf, process->top_table);
  sched_create_thread(process, "idle thread", (void *) process->elf.header.entry, NULL, USER_SPACE);

  return 0;
}
