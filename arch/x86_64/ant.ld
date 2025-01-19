ENTRY(physical_start)

virtual_base = 0xFFFFFFFF80000000;
virtual_entrypoint = virtual_base + 0x1000000;

SECTIONS
{
  . = virtual_entrypoint;
  physical_start = ABSOLUTE(_start - 0xFFFFFFFF80000000);
  
  .boot_info ALIGN(0x1000): AT(ADDR(.boot_info) - virtual_base)
  {
    . += 4096;
  }

  .text ALIGN(0x1000): AT(ADDR(.text) - virtual_base)
  {
    _start_text = .;
    *(.text)
    _end_text = .;
  }

  .data ALIGN(0x1000): AT(ADDR(.data) - virtual_base)
  {
    _start_data = .;
    *(.data)
    _end_data = .;
  }

  .rodata ALIGN(0x1000): AT(ADDR(.rodata) - virtual_base)
  {
    _start_rodata = .;
    *(.rodata)
    _end_rodata = .;
  }

  .bss ALIGN(0x1000): AT(ADDR(.bss) - virtual_base)
  {
    _start_bss = .;
    *(.bss)
    _end_bss = .;
  }

  .brk ALIGN(0x1000): AT(ADDR(.brk) - virtual_base)
  {
    _start_brk = .;
    . += 16 * 1024;
    _end_brk = .;
  }
}
