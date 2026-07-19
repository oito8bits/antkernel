#ifndef _INCLUDE_KEYBOARD_
#define _INCLUDE_KEYBOARD_

#include <ant/types.h>

#define CTRL_MASK 1
#define ALT_MASK 2
#define SHIFT_MASK 3

struct keyboard_event
{
  u8 code;
  char key_char;
  u8 status;
};

void keyboard_irq_handler(void);
void keyboard_get_event(struct keyboard_event *);

#endif
