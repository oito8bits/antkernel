#include <libk/stdlib.h>
#include <libk/string.h>

void utoa(char *source, u64 number)
{
  char *string = source;
  while(number)
  {
    *string++ = number % 10 + '0';
    number /= 10;
  }

  *++string = '\0';

  strrev(source);
}
