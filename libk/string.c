#include <libk/string.h>

size_t strlen(char *string)
{
  char *string_ptr = string;
  
  while(*string++);

  return string - string_ptr - 1; 
}

void strrev(char *string)
{
  char *string_end = string + strlen(string) - 1;
  char character;
  while(string_end > string)
  {
    character = *string;
    *string++ = *string_end;
    *string_end-- = character;
  }
}

void *memcpy(void *dest, const void *src, u64 n)
{
  u8 *p = dest;
  const u8 *q = src;

  while(n--)
    *p++ = *q++;;

  return dest;
}
