#include <lapic.h>
#include <libk/kprintf.h>
#include <arch/map.h>
#include <mm/vmm.h>
#include "msr.h"
#include "pit.h"
#include "io.h"

u32 *lapic_base;

extern struct table_entry kernel_top_table;

static void disable_pic8259a(void)
{
  io_outb(PIC_COMMAND_MASTER, ICW_1);
  io_outb(PIC_COMMAND_SLAVE, ICW_1);
  io_outb(PIC_DATA_MASTER, ICW_2_MASTER);
  io_outb(PIC_DATA_SLAVE, ICW_2_SLAVE);
  io_outb(PIC_DATA_MASTER, ICW_3_MASTER);
  io_outb(PIC_DATA_SLAVE, ICW_3_SLAVE);
  io_outb(PIC_DATA_MASTER, ICW_4);
  io_outb(PIC_DATA_SLAVE, ICW_4);
  io_outb(PIC_DATA_MASTER, 0xff);
  io_outb(PIC_DATA_SLAVE, 0xff);
}

static void timer_init(void)
{

}

void lapic_write_reg(enum lapic_register reg, u32 value)
{
  lapic_base[reg] = value;
}

u32 lapic_read_reg(enum lapic_register reg)
{
  return lapic_base[reg];
}

void lapic_timer_isr(void)
{

}

void lapic_init(void)
{
  lapic_base = (u32 *) KERNEL_LAPIC_BASE;
  map_pages(&kernel_top_table,
            (phys_addr_t) rdmsr(IA32_APIC_BASE) & ~0xfff,
            lapic_base,
            BIT_PRESENT | BIT_WRITE | BIT_CACHE_DISABLE,
            KERNEL_DEFAULT_SIZE / PAGE_SIZE);
  lapic_write_reg(LAPIC_SPURIOUS, lapic_read_reg(LAPIC_SPURIOUS) | (1 << 8) | 0xff);
  disable_pic8259a();
}
