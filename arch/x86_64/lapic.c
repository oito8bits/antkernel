#include <lapic.h>
#include <arch/map.h>
#include <pg.h>
#include "msr.h"
#include "pit.h"
#include <arch/io.h>

void *lapic_base;

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

void lapic_write_reg(enum lapic_register reg, u32 value)
{
  *((u32 *) (lapic_base + reg)) = value;
}

u32 lapic_read_reg(enum lapic_register reg)
{
  return *((u32 *) (lapic_base + reg));
}

static void timer_init(void)
{
  lapic_write_reg(LAPIC_TMRDIV, 0x3);
  lapic_write_reg(LAPIC_TMRINITCNT, 0xffffffff);
  pit_sleep(10);
  lapic_write_reg(LAPIC_LVT_TMR, 0x10000U);
  u32 ticks_per_10ms = 0xffffffff - lapic_read_reg(LAPIC_TMRCURRCNT);
  lapic_write_reg(LAPIC_LVT_TMR, 32 | 1 << 17); /* 1 << 17 = periodic Mode, and 32 = vector */
  lapic_write_reg(LAPIC_TMRINITCNT, ticks_per_10ms);
}

void lapic_timer_isr(void)
{

}

void lapic_eoi(void)
{
  lapic_write_reg(LAPIC_EOI, 0x0);
}

void lapic_init(void)
{
  phys_addr_t apic_phys_base = rdmsr(IA32_APIC_BASE) & ~0xfff;
  lapic_base = pg_phys_to_virt(apic_phys_base);
  map_pages(&kernel_top_table,
            apic_phys_base,
            lapic_base,
            BIT_PRESENT | BIT_WRITE | BIT_CACHE_DISABLE | (1UL << 8),
            KERNEL_DEFAULT_SIZE / PAGE_SIZE);
  lapic_write_reg(LAPIC_SPURIOUS, lapic_read_reg(LAPIC_SPURIOUS) | (1 << 8) | 0xff);
  timer_init();
  disable_pic8259a();
  lapic_eoi();
}
