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

static u64 create_stack(struct table_entry *top_table)
{
  u64 addr = 0x800000000000;
  vmm_map(top_table, (void *) addr - STACK_SIZE, 1, BIT_PRESENT | BIT_USER | BIT_WRITE);
  return addr - 512;
}

static void destroy_stack(struct table_entry *top_table, u64 addr)
{
  //TODO: Unmap stack.
}

__attribute__ ((noinline))
static void save_registers(struct isr_context *new, struct isr_context *old)
{
  new->rip = old->rip; 
  new->rsp = old->rsp;
  new->cs = old->cs;
  new->ss = old->ss;
  /*
  new->r9 = old->r9; 
  new->r10 = old->r10; 
  new->r11 = old->r11; 
  new->r12 = old->r12; 
  new->r13 = old->r13; 
  new->r14 = old->r14; 
  new->r15 = old->r15; 
  new->rbp = old->rbp; 
  new->rbx = old->rbx; 
  new->rax = old->rax; 
  new->rcx = old->rcx; 
  new->rdx = old->rdx; 
  new->rsi = old->rsi; 
  new->rdi = old->rdi; 
  new->rflags = old->rflags; 
  */
}
extern u64 kernel_top_table;
static void switch_top_table(struct sched_thread *thread)
{
  void *top_table = thread->parent->top_table;
  vmm_kappend_kernel_space(top_table); 
  pg_switch_top_table(pg_virt_to_phys(top_table));
/*
  u64 *le = top_table;
  void *addr = (void *) 0x401000;
  addr = (void *) 0x800000000000;
  addr = (void *) 0x4a4000;
  //addr = (void *) 0xFFFFFFFF80000000 + (1 << 29);
  le = pg_phys_to_virt(le[pg_get_l4_idx(addr)] & ~0xfff);
  kprintf("l4e: %lx\n", le);
  le = pg_phys_to_virt(le[pg_get_l3_idx(addr)] & ~0xfff);
  kprintf("l3e: %lx\n", le);
  le = pg_phys_to_virt(le[pg_get_l2_idx(addr)] & ~0xfff);
  kprintf("l2e: %lx\n", le);
  le = pg_phys_to_virt(le[pg_get_l1_idx(addr)] & ~0xfff);
  kprintf("l1e: %lx\n", le);
*/
}

void sched(struct isr_context *context)
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
  save_registers(&current_thread->context, context);
  current_thread->status = READY;
  current_thread = thread;
  current_thread->status = RUNNING;
  save_registers(context, &current_thread->context);
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

extern u64 kernel_top_table;
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
    __asm__ __volatile__("hlt");
  }
}

void sched_init(void)
{
  list_head_init(&processes.head); 
  
  struct sched_process *process = sched_create_process("idle process");
  current_thread = sched_create_thread(process, "main thread", sched_idle, 0, KERNEL_SPACE);  
/*
  struct sched_process *process2 = sched_create_process("idle process");
  sched_create_thread(process2, "main thread", sched_idle2, 0);  
  sched_create_thread(process2, "main thread", sched_idle4, 0);  

  struct sched_process *process3 = sched_create_process("idle process");
  sched_create_thread(process3, "main thread", sched_idle3, 0); 
*/
}
