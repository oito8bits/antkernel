#include <libk/kprintf.h>
#include <stdarg.h>
#include <kernel/console.h>
#include <libk/stdlib.h>

static void print_hex(u64 number, _Bool uppercase)
{
  u8 nibble;
  _Bool zero_num = 0;

  if(number == 0)
  {
    console_put_char('0');
    return;
  }

  size_t i = 16;
  while(i--)
  {
    nibble = number >> 60 & 0xf;
    number <<= 4;
 
    if(!nibble && !zero_num)
      continue;
    else
      zero_num = 1;

    if(nibble < 0xa)
      console_put_char(nibble + '0');
    else if(uppercase)
      console_put_char(nibble - 10 + 'A');
    else
      console_put_char(nibble - 10 + 'a');
  }
}

static void print_uint(u64 number)
{
  char string[21] = {0};

  utoa(string, number);
  console_write(string);
}

static void print_int(s64 number, _Bool is_long)
{
  char string[20] = {0};
  u64 number_long = number;

  if(number < 0 && is_long)
  {
    console_put_char('-');
    number_long = -number;
  }
  
  if((s32) number < 0 && !is_long)
  {
    console_put_char('-');
    number_long = -((s32) number);
  }
  
  utoa(string, number_long);
  
  console_write(string);
}

int kprintf(const char *format, ...)
{
  va_list arg;
  va_start(arg, format);

  while(*format != '\0')
  {
    _Bool is_long = 0;

    if(*format == '%')
    {
      format++;
      if(*format == '#')
      {
        console_write("0x");
        format++;
      }

      if(*format != 'l')
        is_long = 0;
      else
      {
        format++;
        is_long = 1;
      }

      switch(*format)
      {
        case '%':
          console_put_char('%');
          break;
        case 'c':
          console_put_char((u8) va_arg(arg, int));
          break;
        case 's':
          console_write(va_arg(arg, void *));
          break;
        case 'i':
          print_int(va_arg(arg, u64), is_long);
          break;
        case 'u':
          print_uint(va_arg(arg, u64));
          break;
        case 'X':
          print_hex(va_arg(arg, u64), 1);
          break;
        case 'x':
          print_hex(va_arg(arg, u64), 0);
          break;
       }
    }
    else
      console_put_char(*format);

    format++;
  }

  va_end(arg);
}
