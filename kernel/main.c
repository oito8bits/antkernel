#include <mm/pmm.h>
#include <ant/boot.h>
#include <libk/kprintf.h>

int kmain(void)
{
  struct boot_info *boot_info = boot_get_info();
  pmm_init(boot_info);

  return 0;
}
