#include "gdt.h"

extern void gdtr_set(u64);
extern void tr_set_tss(void);

struct tss
{
  u32 reserved0;
  u64 rsp0;
  u64 rsp1;
  u64 rsp2;
  u64 reserved1;
  u64 ist1;
  u64 ist2;
  u64 ist3;
  u64 ist4;
  u64 ist5;
  u64 ist6;
  u64 ist7;
  u64 reserved2;
  u16 reserved3;
  u16 bitmap_offset;
} __attribute__ ((packed));

struct tss tss;

struct gdt_entry gdt[] =
{
  GDT_ENTRY(0x0, 0x0UL, 0x0, 0x0, 0x0),
  GDT_ENTRY(0x0, 0x0UL, 0xa, 0x0, 0x17),
  GDT_ENTRY(0x0, 0x0UL, 0x2, 0x0, 0x1b),
  GDT_ENTRY(0x0, 0x0UL, 0xa, 0x3, 0x17),
  GDT_ENTRY(0x0, 0x0UL, 0x2, 0x3, 0x1b),
  GDT_ENTRY(0x0, 0x0UL, 0x9, 0x0, 0x2)
};

struct gdt_register gdtr;

void gdt_load(void)
{
  //gdt[5] = GDT_ENTRY(&tss, sizeof(tss) - 1, 0x9, 0x0, 0x2);
  u64 base = (u64) &tss;
  u64 limit =  sizeof(tss) - 1;
  gdt[5].lbase = base & 0xffff;
  gdt[5].mbase = base  >> 16 & 0xff;
  gdt[5].hbase = base  >> 32 & 0xffffffff;
  gdt[5].llimit = limit & 0xffff;
  gdt[5].hlimit = limit >> 16 & 0xf;

  gdtr.limit = sizeof(gdt) - 1;
  gdtr.base = (u64) gdt;

  gdtr_set((u64) &gdtr);
  tr_set_tss();
}
