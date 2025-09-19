#include <lapic.h>
#include <libk/kprintf.h>
#include "msr.h"
#include "io.h"

void *lapic_base;

static void disable_pic8259a()
{
  io_out(PIC_COMMAND_MASTER, ICW_1);
  io_out(PIC_COMMAND_SLAVE, ICW_1);
  io_out(PIC_DATA_MASTER, ICW_2_MASTER);
  io_out(PIC_DATA_SLAVE, ICW_2_SLAVE);
  io_out(PIC_DATA_MASTER, ICW_3_MASTER);
  io_out(PIC_DATA_SLAVE, ICW_3_SLAVE);
  io_out(PIC_DATA_MASTER, ICW_4);
  io_out(PIC_DATA_SLAVE, ICW_4);
  io_out(PIC_DATA_MASTER, 0xff);
  io_out(PIC_DATA_SLAVE, 0xff);
}

void lapic_init(void)
{
  lapic_base = (void *) rdmsr(IA32_APIC_BASE);
  disable_pic8259a();
}
