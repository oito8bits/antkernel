#include <mm/early_heap.h>

u8 *brk_addr;
size_t brk_size;

void *early_malloc(size_t size)
{
  void *addr = 0;

  if(size <= brk_size)
  {
    addr = brk_addr;
    brk_addr += size;
    brk_size -= size;
  }

  return addr;
}

void early_heap_init(void *start, size_t size)
{
  brk_addr = start;
  brk_size = size;
}
