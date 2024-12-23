#include "idt.h"
#include <fb/fb.h>
#include "exception.h"

struct idt_entry idt[IDT_ENTRIES];
struct idt_register idtr;

extern u64 ex_handlers;

extern void idtr_set(u64);
extern void exception_handler(void);

void idt_load(void)
{
  int i;
  for(i = 0; i < NUMBER_OF_EXCEPTIONS; i++)
    idt[i] = (struct idt_entry) IDT_ENTRY((u64) (&ex_handlers)[i], 0x8, 0x0, 0xe, 0x0, 0x1);

  idtr.limit = sizeof(idt) - 1;
  idtr.base = (u64) idt;
  idtr_set((u64) &idtr);
}
