#include <arch/x86_64/tss.h>

struct tss tss;

struct tss *tss_get_base(void)
{
  return &tss;
}
