#include <string.h>

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
