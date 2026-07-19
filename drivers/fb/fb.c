#include <fb/fb.h>
#include <mm/vmm.h>
#include <ant/boot.h>
#include "font.h"

static u32 *frame_buffer_base;
static u32 width, height;
static u32 cursor_x, cursor_y;
static u32 background_color;
static u32 foreground_color;

static void plot_pixel(u16 x, u16 y, u32 color)
{
  frame_buffer_base[y * width + x] = color;
}

static u32 get_pixel(u16 x, u16 y)
{
  return frame_buffer_base[y * width + x];
}

static void new_line(void)
{
  cursor_x = 0;
  cursor_y++;
}

static void draw_cursor(void)
{
  size_t i, j;
  size_t x = cursor_x * FB_FONT_WIDTH, y = cursor_y * FB_FONT_HEIGHT;
  size_t old_x = x;
  for(i = 0; i < FB_FONT_HEIGHT; i++)
  {
    for(j = 0; j < FB_FONT_WIDTH; j++)
    {
      u32 pixel = get_pixel(x, y);
      if(pixel == background_color)
        pixel = foreground_color;
      else 
        pixel = background_color;
      
      plot_pixel(x, y, pixel);
      x++;
    }
    x = old_x;
    y++;
  }
}

static void draw_char(u8 c)
{
  u16 i, j;
  u8 bitmap;
  u16 x = cursor_x * 8, y = cursor_y * 16;
  u16 old_x = x;
  u8 no_bitmap = 0;

  if(c < ' ' || c > '~')
  {
    c = '?';
    no_bitmap = 1;
  }

  for(i = 0; i < FB_FONT_HEIGHT; i++)
  {
    bitmap = font8x16[c - ' '][i];

    for(j = 0; j < FB_FONT_WIDTH; j++)
    {
      if(bitmap & (128U >> j))
        plot_pixel(x, y, no_bitmap ? background_color : foreground_color);
      else
        plot_pixel(x, y, no_bitmap ? foreground_color : background_color);

      x++;
    }

    x = old_x;
    y++;
  }
}

static void map_frame_buffer(uintptr_t phys_base)
{
  vmm_map(0, phys_base,
          (void *) FB_VIRTUAL_BASE_ADDR,
          width * height * 4 / PAGE_SIZE,
          KERNEL_DATA);
}

void fb_scrollup(void)
{
  u32 *p = frame_buffer_base + width * FB_FONT_HEIGHT;

  cursor_y--;
  cursor_x = 0;

  size_t i;
  for(i = 0; i < width * (height - FB_FONT_HEIGHT); i++)
    frame_buffer_base[i] = p[i];

  p = frame_buffer_base + width * (height - FB_FONT_HEIGHT);

  for(i = 0; i < width * FB_FONT_HEIGHT; i++)
    p[i] = background_color;
}

void fb_set_background_color(u32 color)
{
  background_color = color;
}

void fb_set_foreground_color(u32 color)
{
  foreground_color = color;
}

u32 fb_get_width(void)
{
  return width;
}

u32 fb_get_height(void)
{
  return height;
}

u32 fb_get_cursor_x(void)
{
  return cursor_x;
}

u32 fb_get_cursor_y(void)
{
  return cursor_y;
}

void fb_put_char(u8 c)
{
  if(c == '\n')
  {
    new_line();
    return;
  }

  if(cursor_y >= height / FB_FONT_HEIGHT)
    fb_scrollup();

  draw_char(c);
  
  if(cursor_x < width / FB_FONT_WIDTH - 1)
    cursor_x++;
  else
    new_line();
}

void fb_write(char *s)
{
  while(*s) fb_put_char(*s++);
}

void fb_init(void)
{
  struct boot_info *boot_info = boot_get_info();

  frame_buffer_base = (u32 *) FB_VIRTUAL_BASE_ADDR;
  width = boot_info->mode.horizontal_resolution;
  height = boot_info->mode.vertical_resolution;
  cursor_x = 0;
  cursor_y = 0;
  fb_set_background_color(FB_BLACK_COLOR);
  fb_set_foreground_color(FB_WHITE_COLOR);
  map_frame_buffer((uintptr_t) boot_info->mode.frame_buffer_base);
}
