#include <ioapic.h>
#include <drivers/acpi/acpi.h>
#include <arch/map.h>
#include <mm/vmm.h>

struct madt_ioapic *madt_ioapic;
u32 *ioapic_base;

extern struct table_entry kernel_top_table;

static u32 read_reg(int reg)
{
  ioapic_base[0] = reg;
  return ioapic_base[4];
}

static void write_reg(int reg, u32 value)
{
  ioapic_base[0] = reg;
  ioapic_base[4] = value;
}

void ioapic_init(void)
{
  ioapic_base = (u32 *) KERNEL_IOAPIC_BASE;
  madt_ioapic = acpi_get_ioapic();
  map_pages(&kernel_top_table,
            (phys_addr_t) madt_ioapic->ioapic_addr,
            ioapic_base,
            BIT_PRESENT | BIT_WRITE | BIT_CACHE_DISABLE | (1UL << 8),
            PAGE_SIZE);
}
