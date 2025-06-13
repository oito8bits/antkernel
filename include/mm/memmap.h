#ifndef _INCLUDE_MEMMAP_
#define _INCLUDE_MEMMAP_

#include <ant/types.h>
#include <ant/boot.h>

enum memory_type
{
  reserved_memory,
  runtime_services_code,
  runtime_services_data,
  available_memory,
  acpi_memory,
  memory_mapped_io,
  memory_mapped_io_port_space,
  unknown_memory
};

size_t memmap_get_memory_pages(struct memory_map *);
enum memory_type memmap_get_memory_type(efi_memory_type);

#endif
