#ifndef _INCLUDE_ACPI_
#define _INCLUDE_ACPI_

#include <ant/types.h>
#include <ant/boot.h>

#define XSDP_SIGNATURE 0x2052545020445352
#define XSDT_SIGNATURE 0x54445358

#define MADT_SIGNATURE 0x7843495041

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

struct lapic
{
  u8 type;
  u8 length;
  u8 acpi_processor_uid;
  u8 acpi_id;
  u8 flags;
} __attribute__ ((packed));

struct ioapic
{
  u8 type;
  u8 length;
  u8 ioapic_id;
  u8 reserved;
  u32 ioapic_address;
  u32 global_sys_interrupt_base;
} __attribute__ ((packed));

void acpi_init(struct boot_info *);

#endif
