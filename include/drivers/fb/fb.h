#ifndef _include_fb_
#define _include_fb_

#include <ant/types.h>
#include <mm/mm.h>

#define FB_WHITE_COLOR 0xffffffff
#define FB_BLACK_COLOR 0x00000000
// MM_KERNEL_VIRTUAL_BASE + 500 MiB
#define FB_VIRTUAL_BASE_ADDR (MM_KERNEL_VIRTUAL_BASE + (1 << 29))

void fb_scrollup(void);
void fb_set_background_color(u32);
void fb_set_foreground_color(u32);
void fb_init(void);
void fb_put_char(u8);
void fb_write(char *);

#endif
