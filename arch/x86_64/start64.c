#include <ant/boot.h>
#include <fb/fb.h>
#include "gdt.h"
#include "heap.h"
#include "idt.h"

extern unsigned long kernel_table_level_2; 

int start64(struct boot_info *info)
{
  early_heap_init();
  gdt_load();
  idt_load();
  fb_init(info, (void *) &kernel_table_level_2);
  return 0;
}
