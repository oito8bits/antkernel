#include <acpi/acpi.h>
#include <arch/map.h>
#include <pg.h>
#include <mm/vmm.h>

struct xsdp *xsdp;
struct sdt *xsdt;
struct sdt *madt;
struct madt_ioapic *ioapic;
phys_addr_t *tables;

bool is_madt(struct sdt *table)
{
  return *((u32 *) table) == MADT_SIGNATURE ? true : false;
}

static bool is_xsdp(struct xsdp *table)
{
  return *((u64 *) table) == XSDP_SIGNATURE ? true : false;
}

static bool is_xsdt(struct sdt *table)
{
  return *((u32 *) table) == XSDT_SIGNATURE ? true : false;
}

void map_sdt_tables(void)
{
  size_t i;
  size_t ntables = (xsdt->length - sizeof(struct sdt)) / 8;
  tables = (void *) xsdt + sizeof(struct sdt);
  struct sdt *table;
  for(i = 0; i < ntables; i++)
  {
    vmm_kmap_data((phys_addr_t) tables[i],
                  pg_phys_to_virt(tables[i]),
                  1);

    table = pg_phys_to_virt(tables[i]);
    
    if(is_madt(table))
      madt = table;

    if(table->length > PAGE_SIZE)
      vmm_kmap_data((phys_addr_t) tables[i],
                    table,
                    table->length / PAGE_SIZE);
  }
}

void madt_parse(void)
{
  struct madt *madt_base = (void *) madt + sizeof(struct sdt) + 8;
  struct madt *entry = madt_base;
  size_t table_len = madt->length - sizeof(struct sdt) - 8;
  size_t i = 0;
  while(i < table_len)
  {
    if(entry->entry_type == 1)
      ioapic = (struct madt_ioapic *) entry;
    i += entry->record_length;
    entry = (void *) madt_base + i;
  }
}

struct madt_ioapic *acpi_get_ioapic(void)
{
  return ioapic;
}

void acpi_init(struct boot_info *boot_info)
{
  phys_addr_t table_phys = (phys_addr_t) boot_info->acpi;
  void *table = pg_phys_to_virt(table_phys);
  vmm_kmap_data(table_phys, table, 1);

  if(is_xsdp(table) == false)
    return;
  
  xsdp = table;

  table_phys = (phys_addr_t) xsdp->xsdt_address;
  table = pg_phys_to_virt(table_phys);
  vmm_kmap_data(table_phys, table, 1);

  if(is_xsdt(table) == false)
    return;

  xsdt = table;
  
  map_sdt_tables();
  madt_parse();
}
