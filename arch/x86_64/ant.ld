ENTRY(physical_start)

virtual_base = 0xFFFFFFFF80000000;
virtual_entrypoint = virtual_base + 0x100000;

SECTIONS
{
  . = virtual_entrypoint;
  physical_start = ABSOLUTE(_start - 0xFFFFFFFF80000000);
  
  .text ALIGN(0x1000): AT(ADDR(.text) - virtual_base)
  {
    _start_text = .;
    *(.text)
    *(.text*)
    _end_text = .;
  }

  .data ALIGN(0x1000): AT(ADDR(.data) - virtual_base)
  {
    _start_data = .;
    *(.data)
    *(.data*)
    _end_data = .;
  }

  .rodata ALIGN(0x1000): AT(ADDR(.rodata) - virtual_base)
  {
    _start_rodata = .;
    *(.rodata)
    *(.rodata*)
    _end_rodata = .;
  }
  
  .stack ALIGN(0x1000): AT(ADDR(.stack) - virtual_base)
  {
    _start_stack = .;
    . += 16 * 1024;
    _end_stack = .;
  }

  .bss ALIGN(0x1000): AT(ADDR(.bss) - virtual_base)
  {
    _start_bss = .;
    *(.bss)
    *(.bss*)
    _end_bss = .;
  }

  .brk ALIGN(0x1000): AT(ADDR(.brk) - virtual_base)
  {
    _start_brk = .;
    . += 16 * 1024;
    _end_brk = .;
  }
}
