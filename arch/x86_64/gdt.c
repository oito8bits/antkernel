#include "gdt.h"
#include "tss.h"

extern void gdtr_set(u64);
extern void tr_set_tss(void);
extern uintptr_t _end_stack;

struct gdt_entry gdt[] =
{
  GDT_ENTRY(0x0, 0x0UL, 0x0, 0x0, 0x0),
  GDT_ENTRY(0x0, 0x0UL, 0xa, 0x0, 0x17),
  GDT_ENTRY(0x0, 0x0UL, 0x2, 0x0, 0x1b),
  GDT_ENTRY(0x0, 0x0UL, 0xa, 0x3, 0x17),
  GDT_ENTRY(0x0, 0x0UL, 0x2, 0x3, 0x1b),
  GDT_ENTRY(0x0, 0x0UL, 0x9, 0x0, 0x2),
  GDT_ENTRY(0x0, 0x0UL, 0x0, 0x0, 0x0),
};

struct gdt_register gdtr;

void gdt_load(void)
{
  struct tss *tss = tss_get_base();
  tss->rsp0 = (u64) &_end_stack - 512;

  u64 base = (u64) tss;
  u64 limit =  sizeof(struct tss) - 1;
  gdt[5].lbase = base & 0xffffff;
  gdt[5].hbase = base  >> 24 & 0xff;
  *((u64 *) &gdt[6]) = base >> 32;
  gdt[5].llimit = limit & 0xffff;
  gdt[5].hlimit = limit >> 16 & 0xf;

  gdtr.limit = sizeof(gdt) - 1;
  gdtr.base = (u64) gdt;

  gdtr_set((u64) &gdtr);
  tr_set_tss();
}
