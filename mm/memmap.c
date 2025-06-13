#include <mm/memmap.h>

size_t memmap_get_memory_pages(struct memory_map *map)
{
  efi_memory_descriptor *descriptor = map->memory_descriptor;
  size_t i;
  size_t pages = 0;
  for(i = 0; i < map->memory_map_size / map->descriptor_size; i++)
  {
    descriptor = (void *) descriptor + map->descriptor_size;
    pages += descriptor->number_of_pages;
  }

  return pages;
}

enum memory_type memmap_get_memory_type(efi_memory_type type)
{
  switch(type)
  {
    case efi_reserved_memory_type:
    case efi_unusable_memory:
    case efi_pal_code:
    case efi_persistent_memory:
    case efi_max_memory_type:
      return reserved_memory;
    case efi_runtime_services_code:
      return runtime_services_code;
    case efi_runtime_services_data:
      return runtime_services_data;
    case efi_conventional_memory:
    case efi_loader_code:
    case efi_loader_data:
    case efi_boot_services_code:
    case efi_boot_services_data:
      return available_memory;
    case efi_acpi_reclaim_memory:
    case efi_acpi_memory_nvs:
      return acpi_memory;
    case efi_memory_mapped_io:
      return memory_mapped_io;
    case efi_memory_mapped_io_port_space:
      return memory_mapped_io_port_space;
  }
}
