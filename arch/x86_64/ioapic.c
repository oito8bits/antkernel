#include <ioapic.h>
#include <arch/int.h>
#include <drivers/acpi/acpi.h>
#include <arch/map.h>
#include <libk/kprintf.h>
#include <mm/vmm.h>

struct madt_ioapic *madt_ioapic;
volatile u32 *ioapic_base;

struct iored
{
  union
  {
    struct
    {
      u8 vector;
      u8 flags;
      u8 mask;
      u32 reserved;
      u8 target;
    };

    struct
    {
      u32 lo;
      u32 hi;
    };

    u64 raw;
  };
} __attribute__ ((packed));

extern struct table_entry kernel_top_table;

static u32 read_reg(u32 reg)
{
  ioapic_base[0] = reg;
  return ioapic_base[4];
}

static void write_reg(u32 reg, u32 value)
{
  ioapic_base[0] = reg;
  ioapic_base[4] = value;
}

static u64 read_redirection(u32 irq)
{
  return ((u64) read_reg(0x11 + 2 * irq) << 32) | read_reg(0x10 + 2 * irq);
}

static void write_redirection(u32 irq, struct iored *iored)
{
  write_reg(0x10 + 2 * irq, iored->lo);
  write_reg(0x11 + 2 * irq, iored->hi);
}

void iored_init(void)
{
  // Set iored entries
  size_t i;
  struct iored iored_entry;
  for(i = 0; i < 2; i++)
  {
     iored_entry.raw = read_redirection(i);
     iored_entry.vector = IRQ_IOAPIC_BASE + i;
     iored_entry.flags = 0;
     iored_entry.mask = 1;
     iored_entry.target = 0;
     write_redirection(i, &iored_entry);
  }

  // Unmask keyboard.
  iored_entry.raw = read_redirection(1);
  iored_entry.mask = 0;
  write_redirection(1, &iored_entry);
}

void ioapic_init(void)
{
  ioapic_base = (u32 *) KERNEL_IOAPIC_BASE;
  madt_ioapic = acpi_get_ioapic();
  
  // Map ioapic register area.
  map_pages(&kernel_top_table,
            (phys_addr_t) madt_ioapic->ioapic_addr,
            (void *) ioapic_base,
            BIT_PRESENT | BIT_WRITE | BIT_CACHE_DISABLE | (1UL << 8),
            PAGE_SIZE);

  iored_init();
}
