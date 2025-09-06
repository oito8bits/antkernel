#include <ant/boot.h>
#include <fb/fb.h>
#include <libk/kprintf.h>
#include "gdt.h"
#include <mm/early_heap.h>
#include "idt.h"

extern unsigned long kernel_table_level_2; 

int start64(void)
{
  struct boot_info *boot_info = boot_get_info();

  early_heap_init();
  gdt_load();
  idt_load();
  return 0;
}
