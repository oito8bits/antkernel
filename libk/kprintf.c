#include <libk/kprintf.h>
#include <stdarg.h>
#include <fb/fb.h>
#include <libk/stdlib.h>

static void print_hex(u64 number, _Bool uppercase)
{
  u8 nibble;
  _Bool zero_num = 0;

  if(number == 0)
  {
    fb_put_char('0');
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
      fb_put_char(nibble + '0');
    else if(uppercase)
      fb_put_char(nibble - 10 + 'A');
    else
      fb_put_char(nibble - 10 + 'a');
  }
}

static void print_uint(u64 number)
{
  char string[21] = {0};

  utoa(string, number);
  fb_write(string);
}

static void print_int(s64 number, _Bool is_long)
{
  char string[20] = {0};
  u64 number_long = number;

  if(number < 0 && is_long)
  {
    fb_put_char('-');
    number_long = -number;
  }
  
  if((s32) number < 0 && !is_long)
  {
    fb_put_char('-');
    number_long = -((s32) number);
  }
  
  utoa(string, number_long);
  
  fb_write(string);
}

int kprintf(const char *format, ...)
{
  va_list arg;
  va_start(arg, format);

  while(*format != '\0')
  {
    u64 long_arg;
    _Bool is_long = 0;

    if(*format == '%')
    {
      format++;
      if(*format == '#')
      {
        fb_write("0x");
        format++;
      }

      if(*format != 'l')
      {
        long_arg = va_arg(arg, u32);
        is_long = 0;
      }
      else
      {
        long_arg = va_arg(arg, u64);
        format++;
        is_long = 1;
      }

      switch(*format)
      {
        case '%':
          fb_put_char('%');
          break;
        case 'c':
          fb_put_char((u8) va_arg(arg, int));
          break;
        case 's':
          fb_write(va_arg(arg, void *));
          break;
        case 'i':
          print_int(long_arg, is_long);
          break;
        case 'u':
          print_uint(long_arg);
          break;
        case 'X':
          print_hex(long_arg, 1);
          break;
        case 'x':
          print_hex(long_arg, 0);
          break;
       }
    }
    else
      fb_put_char(*format);

    format++;
  }

  va_end(arg);
}
