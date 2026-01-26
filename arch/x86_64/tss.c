#include "tss.h"

struct tss tss;

struct tss *tss_get_base(void)
{
  return &tss;
}
