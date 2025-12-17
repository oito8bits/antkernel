#include <kernel/sched/sched.h>
#include <libk/string.h>
#include <mm/heap.h>
#include <mm/vmm.h>
#include <libk/kprintf.h>

//#define STACK_SIZE (1 << 14)
#define STACK_SIZE 4096

struct sched_process processes;
struct sched_thread *current_thread;
size_t last_free_pid;
size_t last_free_tid;

unsigned long count1;
unsigned long count2;
unsigned long count3;
void sched_idle(void);
void sched_idle2(void);

static u64 create_stack(struct table_entry *top_table)
{
  u64 addr = KERNEL_BASE - STACK_SIZE;
  vmm_map(top_table, (void *) addr, STACK_SIZE, BIT_PRESENT | BIT_WRITE | BIT_USER);
  
  return addr;
}

static void destroy_stack(struct table_entry *top_table, u64 addr)
{
  //TODO: Unmap stack.
}

static void switch_context(struct isr_context *new, struct isr_context *old)
{
  new->rip = old->rip;
  new->rsp = old->rsp;
}

void sched(struct isr_context *context)
{
  if(current_thread != 0)
    //current_thread->context.rip = context->rip;
    switch_context(&current_thread->context, context);
  else
  {
    struct sched_process *process = (struct sched_process *) processes.head.next;
    current_thread = (struct sched_thread *) &process->threads.head;
  }

  struct sched_thread *thread = (struct sched_thread *) current_thread->head.next;
  if(list_is_head(&thread->head, &current_thread->parent->threads.head))
  {
    struct sched_process *next_process = (struct sched_process *) current_thread->parent->head.next;
    if(list_is_head(&next_process->head, &processes.head))
      next_process = (struct sched_process *) next_process->head.next;
    thread = (struct sched_thread *) next_process->threads.head.next;
  }
  
  current_thread->status = READY;
  current_thread = thread;
  current_thread->status = RUNNING;
  //context->rip = current_thread->context.rip;
  switch_context(context, &current_thread->context);
}

struct sched_process *sched_create_process(const char *name)
{
  struct sched_process *new_process = heap_malloc(sizeof(struct sched_process));

  new_process->pid = last_free_pid++;

  size_t table_size = sizeof(struct table_entry) * 512;
  new_process->top_table = heap_malloc(table_size);
  kprintf("process top table: %lx\n", new_process->top_table);
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
  heap_free(process->top_table);
  heap_free(process);
}

extern u64 kernel_top_table;
struct sched_thread *sched_create_thread(struct sched_process *process, const char *name, void *entrypoint, void *arg)
{
  struct sched_thread *thread = heap_malloc(sizeof(struct sched_thread));
  if(thread == 0)
    return 0;

  thread->tid = last_free_tid++;
  thread->status = READY;
  thread->context.ss = 0x10;
  //thread->context.rsp = create_stack((void *) &kernel_top_table);
  thread->context.rsp = (u64) heap_malloc(256);
  kprintf("rsp: %lx\n", thread->context.rsp);
  //heap_show_blocks();
  thread->context.rflags = 0x202;
  thread->context.cs = 0x8;
  thread->context.rip = (u64) entrypoint;
  thread->context.rdi = (u64) arg;
  thread->context.rbp = 0;
  //thread->rsp0 = heap_malloc(STACK_SIZE);
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
    count1++;
    __asm__ __volatile__("hlt");
  }
}

void sched_idle2(void)
{
  while(1)
  {
    count2++;
    __asm__ __volatile__("hlt");
  }
}

void sched_idle3(void)
{
#include <drivers/fb/fb.h>
  while(1)
  {
    //kprintf("count1: %li count2: %li, count3: %li\n", count1, count2, count3);
    fb_write("shed_idle3...\n");
    __asm__ __volatile__("hlt");
  }
}

void sched_idle4(void)
{
  while(1)
  {
    count3++;
    __asm__ __volatile__("hlt");
  }
}

void sched_init(void)
{
  list_head_init(&processes.head);
  
  struct sched_process *process = sched_create_process("idle process");
  sched_create_thread(process, "main thread", sched_idle, 0);  
  
  struct sched_process *process2 = sched_create_process("idle process");
  sched_create_thread(process2, "main thread", sched_idle2, 0);  
  sched_create_thread(process2, "main thread", sched_idle4, 0);  

  struct sched_process *process3 = sched_create_process("idle process");
  sched_create_thread(process3, "main thread", sched_idle3, 0);  
}
