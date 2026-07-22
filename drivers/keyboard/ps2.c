#include <drivers/keyboard/ps2.h>
#include <libk/kprintf.h>
#include <string.h>
#include <arch/io.h>

#define NORMAL_STATE 0
#define PREFIX_STATE 1

static u8 current_state;
static struct keyboard_event event_buf[255];
static volatile int key_position = 0;

char key_char[] = 
{
  0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
  '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u',
  'i', 'o', 'p', '[', ']', '\n', '0', 'a', 's', 'd', 'f',
  'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z',
  'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0,
  ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-',
  0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void keyboard_get_event(struct keyboard_event *event)
{
  int pos = key_position;

  int key_code;
  while(1)
  {
    key_code = event_buf[key_position - 1].code;

    if(pos != key_position && key_code > 0 && key_code < sizeof(key_char))
      break;
  }

  memcpy(event, &event_buf[key_position - 1], sizeof(struct keyboard_event));
}

void keyboard_irq_handler(void)
{
  int scancode = io_inb(0x60);
 
  if(scancode == 0xe0)
  {
    current_state = PREFIX_STATE;
    return;
  }
  if(current_state == PREFIX_STATE)
    current_state = NORMAL_STATE;

  event_buf[key_position].code = scancode;
  event_buf[key_position].key_char = key_char[scancode];
  key_position = (key_position + 1) % 255;
}

void init_keyboard(void)
{
  current_state = NORMAL_STATE;
}
