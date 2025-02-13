#include <mm/zones.h>
#include <ant/boot.h>

extern struct boot_info boot_info;

int kmain(void)
{
  struct boot_info *boot_info = boot_get_info();
  zones_init(&boot_info->map);
  return 0;
}
