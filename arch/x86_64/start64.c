#include <ant/boot.h>
#include <fb/fb.h>
#include <libk/kprintf.h>
#include "gdt.h"
#include <mm/early_heap.h>
#include "idt.h"

extern u8 _start_brk;
extern u8 _end_brk;

int start64(void)
{
  struct boot_info *boot_info = boot_get_info();

  early_heap_init(&_start_brk, &_end_brk - &_start_brk);
  gdt_load();
  idt_load();
  return 0;
}
