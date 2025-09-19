#ifndef _INCLUDE_IO_
#define _INCLUDE_IO_

void io_out(unsigned short port, unsigned char value)
{
	__asm__ __volatile__(
			"outb %%al, %%dx"
			: : "r" (value), "r" (port));
}

#endif
