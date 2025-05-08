#include <mm/addr.h>

void *addr_phys_to_virt(void *phys_addr)
{
  return VIRTUAL_BASE + phys_addr;
}
