#ifndef _INCLUDE_IO_
#define _INCLUDE_IO_

#include <ant/types.h>

static inline void io_outb(u16 port, u8 value)
{
	__asm__ __volatile__(
			"outb %0, %1"
			: : "a" (value), "Nd" (port));
}

static inline u16 io_inb(u16 port)
{
  u8 result;

  __asm__ __volatile__(
      "inb %1, %0"
      : "=a" (result): "Nd" (port));

  return result;
}

#endif
