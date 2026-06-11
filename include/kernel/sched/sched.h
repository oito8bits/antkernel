#ifndef _INCLUDE_SCHED_
#define _INCLUDE_SCHED_

#include <ant/types.h>
#include <ant/list.h>
#include <arch/x86_64/context.h>
#include <kernel/elf.h>
#include <mm/vmm.h>

#define FORK    (1UL << 0)
#define READY   (1UL << 1)
#define RUNNING (1UL << 2)
#define DEAD    (1UL << 3)

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
  void *rsp;
  u64 status;
  struct sched_process *parent;
};

struct sched_process
{
  struct list_head head;
  char *path;
  size_t pid;
  struct table_entry *top_table; 
  struct sched_thread threads;
  u64 status;
  struct elf_64 elf;
};

void sched(struct context *context);
struct sched_process *sched_create_process(const char *, u64);
void sched_add_process(struct sched_process *);
struct sched_process *sched_copy_process(struct sched_process *, struct context *);
void sched_destroy_process(struct sched_process *);
void sched_exit(struct sched_process *);
void sched_destroy_curr_process(void);
struct sched_thread *sched_create_thread(struct sched_process *, const char *, void *, void *, enum sched_space_type);
void sched_destroy_thread(struct sched_thread *);
void sched_destroy_threads(struct sched_thread *);
struct sched_process *sched_get_current_process(void);
void sched_init(void);

#endif
