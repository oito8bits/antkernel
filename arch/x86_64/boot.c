#include <ant/boot.h>
#include <ant/mm.h>
#include <libk/string.h>

struct boot_info boot_info;
efi_memory_descriptor map[4096];

virt_addr_t boot_init(struct boot_info *info)
{
  memcpy(&boot_info, info, sizeof(struct boot_info));
  memcpy(&map, &info->map.memory_descriptor, info->map.memory_map_size);
  boot_info.map.memory_descriptor = map + MM_KERNEL_VIRTUAL_BASE;

  return (uintptr_t) &boot_info + MM_KERNEL_VIRTUAL_BASE;
}
