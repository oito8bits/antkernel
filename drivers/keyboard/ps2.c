#include <libk/kprintf.h>
#include <arch/io.h>

void keyboard_irq_handler(void)
{
  int scancode = io_inb(0x60);
  kprintf("Scancode read: %i\n", scancode);
}

