#include <kernel/console.h>
#include <drivers/fb/fb.h>
#include <mm/heap.h>
#include <kernel/lock.h>

static u8 *buffer;
static u32 width;
static u32 height;

static struct lock console_lock;

static void put_char(char c)
{
  size_t position = fb_get_cursor_y() * width + fb_get_cursor_x();
  *(buffer + position) = c;
  fb_put_char(c);
}

void console_put_char(char c)
{
  lock_acquire(&console_lock);
  put_char(c);
  lock_release(&console_lock);
}

void console_write(char *str)
{
  lock_acquire(&console_lock);
  while(*str) put_char(*str++); 
  lock_release(&console_lock);
}
/*
void console_backspace(void)
{
  size_t x = fb_get_cursor_x();
  size_t i;
  for(i = x; buffer[i] != '\n'; i++)
  {
    
  }
}
*/
void console_init(void)
{
  width = fb_get_width() / FB_FONT_WIDTH;
  height = fb_get_height() / FB_FONT_HEIGHT;
  buffer = heap_malloc(width * height);
}
