#include <ant/boot.h>
#include <ant/mm.h>
#include <libk/string.h>

static struct boot_info boot_info;
efi_memory_descriptor map[4096];

void boot_init(struct boot_info *info)
{
  memcpy(&boot_info, info, sizeof(struct boot_info));
  memcpy(map, info->map.memory_descriptor, info->map.memory_map_size);
  boot_info.map.memory_descriptor = map;
  boot_info.map.memory_map_size = info->map.memory_map_size;
}

void *boot_get_info(void)
{
  return &boot_info;
}
