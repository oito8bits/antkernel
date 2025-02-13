#ifndef _INCLUDE_BOOT_
#define _INCLUDE_BOOT_

#include "types.h"

typedef enum
{
  efi_reserved_memory_type,
  efi_loader_code,
  efi_loader_data,
  efi_boot_services_code,
  efi_boot_services_data,
  efi_runtime_services_code,
  efi_runtime_services_data,
  efi_conventional_memory,
  efi_unusable_memory,
  efi_acpi_reclaim_memory,
  efi_acpi_memory_nvs,
  efi_memory_mapped_io,
  efi_memory_mapped_io_port_space,
  efi_pal_code,
  efi_persistent_memory,
  efi_max_memory_type
} efi_memory_type;

typedef struct
{
  u32 type;
  u64 physical_start;
  u64 virtual_start;
  u64 number_of_pages;
  u64 attributes;
} efi_memory_descriptor;

typedef enum
{
  pixel_red_green_blue_reserved_8bit_per_color,
  pixel_blue_green_red_reserved_8bit_per_color,
  pixel_bit_mask,
  pixel_blt_only,
  pixel_format_max
} efi_graphics_pixel_format;

struct gop_video_mode
{
  void                      *frame_buffer_base;
  u64                       frame_buffer_size;
  u32                       horizontal_resolution;
  u32                       vertical_resolution;
  efi_graphics_pixel_format pixel_format;
};

struct memory_map
{
  u64 memory_map_size;
  efi_memory_descriptor *memory_descriptor;
  u64 descriptor_size;
};

struct boot_info
{
  struct memory_map map;    
  struct gop_video_mode mode;
};

void boot_init(struct boot_info *);
void *boot_get_info(void);

#endif
