#include <kernel/sched/sched.h>
#include <libk/string.h>
#include <mm/heap.h>
#include <mm/vmm.h>
#include <libk/kprintf.h>

struct sched_process processes;
struct sched_thread *current_thread;
size_t last_free_pid;
size_t last_free_tid;

static u64 create_stack(struct table_entry *top_table)
{
  vmm_map(top_table, (void *) USER_STACK_BASE, STACK_SIZE / PAGE_SIZE, BIT_PRESENT | BIT_WRITE | BIT_USER);
  
  return USER_STACK_TOP;
}

static void destroy_stack(struct table_entry *top_table, u64 addr)
{
  //TODO: Unmap stack.
}

static void switch_top_table(struct sched_thread *thread)
{
  void *top_table = thread->parent->top_table;
  vmm_kappend_kernel_space(top_table); 
  pg_switch_top_table(pg_virt_to_phys(top_table));
}

void sched(struct context *context)
{
  struct sched_thread *thread;

  thread = (struct sched_thread *) current_thread->head.next;
  if(list_is_head(&thread->head, &current_thread->parent->threads.head))
  {
    struct sched_process *next_process = (struct sched_process *) current_thread->parent->head.next;
    if(list_is_head(&next_process->head, &processes.head))
      next_process = (struct sched_process *) next_process->head.next;
    thread = (struct sched_thread *) next_process->threads.head.next;
  }
  context_copy(&current_thread->context, context);
  current_thread->status = READY;
  current_thread = thread;
  current_thread->status = RUNNING;
  context_copy(context, &current_thread->context);
  context_set_kstack(current_thread->rsp0);
  switch_top_table(current_thread);
}

struct sched_process *sched_create_process(const char *name)
{
  struct sched_process *new_process = heap_malloc(sizeof(struct sched_process));

  new_process->pid = last_free_pid++;

  size_t table_size = sizeof(struct table_entry) * 512;
  new_process->top_table = pg_phys_to_virt(pmm_alloc_table());
  memset(new_process->top_table, 0, table_size);
  if(new_process->top_table == 0)
    return 0;
  
  list_add(&new_process->head, &processes.head);
  list_head_init(&new_process->threads.head);

  return new_process;
}

void sched_destroy_process(struct sched_process *process)
{
  list_del(&process->head);
  pmm_free_table(pg_virt_to_phys(process->top_table));
  heap_free(process);
}

struct sched_thread *sched_create_thread(struct sched_process *process, const char *name, void *entrypoint, void *arg, enum sched_space_type type)
{
  struct sched_thread *thread = heap_malloc(sizeof(struct sched_thread));
  if(thread == 0)
    return 0;

  switch(type)
  {
    case KERNEL_SPACE:
      thread->context.ss = 0x10 | 3;
      thread->context.cs = 0x8 | 3;
      break;
    case USER_SPACE:
      thread->context.ss = 0x20 | 3;
      thread->context.cs = 0x18 | 3;
      break;
  }

  thread->tid = last_free_tid++;
  thread->status = READY;
  thread->context.rsp = create_stack(process->top_table);
  thread->context.rflags = 0x202;
  thread->context.rip = (u64) entrypoint;
  thread->context.rdi = (u64) arg;
  thread->context.rbp = 0;
  thread->rsp0 = heap_malloc(UNIT_KiB(16)) + UNIT_KiB(16);
  //thread->rsp0 = heap_malloc(UNIT_KiB(4)) + UNIT_KiB(4);
  kprintf("thread->rsp0: %lx\n", thread->rsp0);
  thread->parent = process;

  list_add(&thread->head, &process->threads.head);

  return thread;
}

void sched_destroy_thread(struct sched_thread *thread)
{
  list_del(&thread->head);
  destroy_stack(thread->parent->top_table, thread->context.rsp);
  heap_free(thread);
}

void sched_idle(void)
{
  while(1)
  {
    __asm__ __volatile__("hlt");
  }
}

void sched_init(void)
{
  list_head_init(&processes.head); 
  
  struct sched_process *process = sched_create_process("idle process");
  current_thread = sched_create_thread(process, "main thread", sched_idle, 0, KERNEL_SPACE);  
}
