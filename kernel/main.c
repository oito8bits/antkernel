#include <mm/pmm.h>
#include <mm/vmm.h>
#include <ant/boot.h>

extern u64 top_table_level;

int kmain(void)
{
  struct boot_info *boot_info = boot_get_info();
  pmm_init(boot_info);
  vmm_init(boot_info, (struct pde *) &top_table_level);
  
  return 0;
}
