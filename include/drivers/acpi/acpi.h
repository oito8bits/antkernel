#ifndef _INCLUDE_ACPI_
#define _INCLUDE_ACPI_

#include <ant/types.h>
#include <ant/boot.h>

#define XSDP_SIGNATURE 0x2052545020445352
#define XSDT_SIGNATURE 0x54445358

#define MADT_SIGNATURE 0x43495041

enum madt_type
{
  LAPIC,
  IOAPIC,
  IOAPIC_INT_SOURCE_OVERRIDE,
  IOAPIC_NMI_SOURCE,
  LAPIC_NMI,
  LAPIC_ADDRESS_OVERRIDE,
  PROCESSOR_LOCAL_APIC_X2APIC
};

struct rsdp
{
  char signature[8];
  u8 checksum;
  char oemid[6];
  u8 revision;
  u32 rsdt_address;
} __attribute__ ((packed));

struct xsdp
{
  char  signature[8];
  u8    checksum;
  char  oemid[6];
  u8    revision;
  u32   rsdt_address;
  u32   length;
  u64   xsdt_address;
  u8    extended_checksum;
  u8    reserved[3];
} __attribute__ ((packed));

struct sdt
{
  char  signature[4];
  u32   length;
  u8    revision;
  u8    checksum;
  char  oemid[6];
  char  oem_table_id[8];
  u32   oem_revision;
  u32   creator_id;
  u32   creator_revision;
} __attribute__ ((packed));

struct madt
{
  u8 entry_type;
  u8 record_length;
} __attribute__ ((packed));

struct madt_lapic
{
  struct madt madt;
  u8 acpi_processor_id;
  u8 acpi_id;
  u32 flags;
} __attribute__ ((packed));

struct madt_ioapic
{
  struct madt madt;
  u8 ioapic_id;
  u8 reserved;
  u32 ioapic_addr;
  u32 global_sys_int_base;
} __attribute__ ((packed));

struct madt_apic
{
  u32 local_apic_addr;
  u32 flags;
} __attribute__ ((packed));

struct madt_ioapic *acpi_get_ioapic(void);
void acpi_init(struct boot_info *);

#endif
