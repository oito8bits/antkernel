#ifndef _INCLUDE_SCHED_
#define _INCLUDE_SCHED_

#include <ant/types.h>
#include <ant/list.h>
#include <arch/x86_64/context.h>
#include <kernel/elf.h>
#include <mm/vmm.h>

enum process_status
{
  READY,
  RUNNING,
  DEAD
};

enum sched_space_type
{
  KERNEL_SPACE,
  USER_SPACE,
};

struct sched_thread
{
  struct list_head head;
  size_t tid;
  struct context context;
  void *rsp0;
  enum process_status status;
  struct sched_process *parent;
};

struct sched_process
{
  struct list_head head;
  size_t pid;
  struct table_entry *top_table; 
  struct sched_thread threads;
  enum process_status status;
  struct elf_64 elf;
};

void sched(struct context *context);
struct sched_process *sched_create_process(const char *);
void sched_destroy_process(struct sched_process *);
struct sched_thread *sched_create_thread(struct sched_process *, const char *, void *, void *, enum sched_space_type);
void sched_destroy_thread(struct sched_thread *);
void sched_init(void);

#endif
