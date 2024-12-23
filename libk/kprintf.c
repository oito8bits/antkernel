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

static void print_int(s32 number)
{
  char string[22] = {0};
  itoa(string, number);
  fb_write(string);
}

int kprintf(const char *format, ...)
{
  va_list arg;
  va_start(arg, format);

  while(*format != '\0')
  {
    u64 long_arg;

    if(*format == '%')
    {
      format++;
      if(*format == '#')
      {
        fb_write("0x");
        format++;
      }

      if(*format != 'l')
        long_arg = va_arg(arg, unsigned);
      else
      {
        long_arg = va_arg(arg, long unsigned);
        format++;
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
          print_int(long_arg);
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
