#include "cr.h"

void cr_set_cr3(u64 top_table)
{
  __asm__ volatile("mov    %0, %%cr3" : : "r"(top_table));
}
