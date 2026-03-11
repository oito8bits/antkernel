#include <drivers/keyboard/ps2.h>
#include <libk/kprintf.h>
#include <arch/io.h>

#define NORMAL_STATE 0
#define PREFIX_STATE 1

static u8 current_state;
static struct keyboard_event key_buf[255];
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

char keyboard_get_char(void)
{
  int pos = key_position;

  int key_code;
  while(1)
  {
    key_code = key_buf[key_position - 1].code;

    if(pos != key_position && key_code > 0 && key_code < sizeof(key_char))
      break;
  }

  return key_char[key_code];
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

  key_buf[key_position].code = scancode;
  key_position = (key_position + 1) % 255;
}

void init_keyboard(void)
{
  current_state = NORMAL_STATE;
}
