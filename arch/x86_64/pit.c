#include "pit.h"
#include <arch/io.h>

void pit_sleep(u32 ms)
{
  io_outb(0x43, 0x30);
  u16 count = 1193;
  while(ms)
  {
    io_outb(0x40, count & 0xff);
    io_outb(0x40, (count >> 8) & 0xff);
    do
    {
      io_outb(0x43, 0xe2);
    } while(!(io_inb(0x40) & (1 << 7)));
    ms--;
  }
}
