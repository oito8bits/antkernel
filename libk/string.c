#include <libk/string.h>

size_t strlen(const char *string)
{
  const char *string_ptr = string;
  
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

void *memset(void *buff, u64 c, u64 n)
{
  u8 *p = buff;

  while(n--)
    *p++ = c;

  return buff;
}

size_t memcmp(const void *buf1, const void *buf2, size_t count)
{
  const char *p = buf1;
  const char *q = buf2;

  while(count--)
  { 
    if(*p++ != *q++)
      return p[-1] < q[-1] ? -1 : 1;  
  } 
    
  return 0;
}

void *strcpy(char *dest, const char *src)
{
  return memcpy(dest, src, strlen(src) + 1);
}

size_t strncmp(const char *s1, const char *s2, size_t n)
{
  return memcmp(s1, s2, n);
}

