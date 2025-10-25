#include <ioapic.h>
#include <arch/int.h>
#include <drivers/acpi/acpi.h>
#include <arch/map.h>
#include <libk/kprintf.h>
#include <mm/vmm.h>

struct madt_ioapic *madt_ioapic;
u32 *ioapic_base;

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
    u64 raw_iored;
  };
} __attribute__((packed));

extern struct table_entry kernel_top_table;

__attribute__((noinline))
static u32 read_reg(int reg)
{
  ioapic_base[0] = reg;
  return ioapic_base[4];
}

__attribute__((noinline))
static void write_reg(int reg, u32 value)
{
  kprintf("value: %lx\n", value);
  ioapic_base[0] = reg;
  ioapic_base[4] = value;
}

__attribute__((noinline))
static u64 read_redirection(int irq)
{
  u64 lo = read_reg(0x10 + 2 * irq);
  u64 hi = read_reg(0x11 + 2 * irq);
  return hi << 32 | lo;
}

__attribute__((noinline))
static void write_redirection(int irq, struct iored *iored)
{
  write_reg(0x10 + 2 * irq, iored->raw_iored);
  write_reg(0x11 + 2 * irq, iored->raw_iored >> 32);
  //kprintf("read_redirection(): %lx, raw_iored: %lx\n", read_redirection(irq), iored->raw_iored);
    //while(1);
}

void ioapic_init(void)
{
  ioapic_base = (u32 *) KERNEL_IOAPIC_BASE;
  madt_ioapic = acpi_get_ioapic();
  
  // Map ioapic register area.
  map_pages(&kernel_top_table,
            (phys_addr_t) madt_ioapic->ioapic_addr,
            ioapic_base,
            BIT_PRESENT | BIT_WRITE | BIT_CACHE_DISABLE | (1UL << 8),
            PAGE_SIZE);

  // Set iored entries.
  size_t i;
  struct iored iored;
  for(i = 0; i < 2; i++)
  {
     iored.raw_iored = read_redirection(i);
     iored.vector = IRQ_IOAPIC_BASE + i;
     iored.flags = 0x0;
     iored.mask |= 1;
     iored.target = 0x0;
     write_redirection(i, &iored);
  }

  // Unmask keyboard.
  iored.raw_iored = read_redirection(1);
  iored.mask &= ~0x1;
  write_redirection(1, &iored);
}
