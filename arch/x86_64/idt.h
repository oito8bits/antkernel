#ifndef _INCLUDE_IDT_
#define _INCLUDE_IDT_

#include <ant/types.h>

#define IDT_ENTRIES 256

#define IDT_ENTRY(offset, segment_selector, ist, type, dpl, p)  \
{ \
  (offset), \
  segment_selector, \
  ist, \
  0, \
  type, \
  0, \
  dpl, \
  p, \
  (offset) >> 16, \
  (offset) >> 32, \
  0 \
}

struct idt_entry
{
  u16 loffset;
  u16 segment_selector;
  u32 ist: 3;
  u32 zero: 5;
  u32 type: 4;
  u32 zero2: 1;
  u32 dpl: 2;
  u32 p: 1;
  u16 moffset;
  u32 hoffset;
  u32 reserved;
} __attribute__((packed));

struct idt_register
{
  u16 limit;
  u64 base;  
} __attribute__((packed));

void idt_load(void);

#endif
