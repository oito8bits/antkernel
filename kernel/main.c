#include <mm/zones.h>
#include <ant/boot.h>

int kmain(struct boot_info *info)
{
  zones_init(&info->map);
  return 0;
}
