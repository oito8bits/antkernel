#ifndef _INCLUDE_KEYBOARD_
#define _INCLUDE_KEYBOARD_

#include <ant/types.h>

#define CTRL_MASK 1
#define ALT_MASK 2
#define SHIFT_MASK 3

struct keyboard_event
{
  u8 code;
  u8 status;
};

void keyboard_irq_handler(void);
char keyboard_get_char(void);

#endif
