#include <ant/boot.h>
#include <paging.h>
#include <fb/fb.h>
#include <gdt.h>
#include <idt.h>
#include <heap.h>
#include <kprintf.h>

int start64(struct boot_info *info)
{
  early_heap_init();
  paging_init();
  fb_init(info);
  gdt_load();
  idt_load();
  return 0;
}
