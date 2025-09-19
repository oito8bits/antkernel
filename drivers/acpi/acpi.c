#include <acpi/acpi.h>
#include <libk/kprintf.h>
#include <arch/map.h>
#include <mm/vmm.h>

struct xsdp *xsdp;
struct sdt *xsdt;
struct sdt *madt;
phys_addr_t *tables;

extern u8 kernel_top_table;

static bool is_madt(struct sdt *madt)
{
  return *((u64 *) madt) == MADT_SIGNATURE ? true : false;
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
                  (void *) tables[i] + KERNEL_ACPI_BASE,
                  1);

    table = (void *) tables[i] + KERNEL_ACPI_BASE;
    
    if(is_madt(table))
      madt = table;

    if(table->length > PAGE_SIZE)
      vmm_kmap_data((phys_addr_t) tables[i],
                    table,
                    table->length / PAGE_SIZE);
  }
}

void acpi_init(struct boot_info *boot_info)
{
  phys_addr_t table_phys = (phys_addr_t) boot_info->acpi;
  void *table = boot_info->acpi + KERNEL_ACPI_BASE;
  vmm_kmap_data(table_phys, table, 1);

  if(is_xsdp(table) == false)
    return;
  
  xsdp = table;

  table_phys = (phys_addr_t) xsdp->xsdt_address;
  table = (void *) table_phys + KERNEL_ACPI_BASE;
  vmm_kmap_data(table_phys, table, 1);

  if(is_xsdt(table) == false)
    return;

  xsdt = table;
  
  map_sdt_tables();
}
