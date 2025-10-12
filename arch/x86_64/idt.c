#include "idt.h"
#include "isr.h"

struct idt_entry idt[IDT_ENTRIES];
struct idt_register idtr;

extern u64 isr_handlers;

extern void idtr_set(u64);

void idt_load(void)
{
  int i;
  for(i = 0; i < 33; i++)
    idt[i] = (struct idt_entry) IDT_ENTRY((u64) (&isr_handlers)[i], 0x8, 0x0, 0xe, 0x0, 0x1);

  idtr.limit = sizeof(idt) - 1;
  idtr.base = (u64) idt;
  idtr_set((u64) &idtr);
}
