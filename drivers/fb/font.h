#ifndef _INCLUDE_FONT_
#define _INCLUDE_FONT_

const unsigned char font8x16[32][16] = {
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  {0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x00}, // '!'
  {0x6c, 0x6c, 0x6c, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // '"'
  {0x00, 0x24, 0x24, 0x24, 0x24, 0x24, 0x7e, 0x24, 0x24, 0x7e, 0x24, 0x24, 0x24, 0x24, 0x24, 0x00}, // '#'
  {0x00, 0x18, 0x18, 0x3c, 0x42, 0x40, 0x40, 0x40, 0x3c, 0x02, 0x02, 0x42, 0x3c, 0x18, 0x18, 0x00}, // '$'
  {0x00, 0x02, 0x02, 0x3a, 0x2c, 0x3c, 0x04, 0x08, 0x08, 0x08, 0x10, 0x1e, 0x1a, 0x2e, 0x20, 0x00}, // '%'
  {0x00, 0x18, 0x24, 0x24, 0x24, 0x24, 0x18, 0x30, 0x4a, 0x4a, 0x46, 0x46, 0x42, 0x46, 0x3a, 0x00}, // '&'
  {0x18, 0x18, 0x18, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // '''
  {0x00, 0x06, 0x08, 0x10, 0x20, 0x20, 0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 0x10, 0x08, 0x06, 0x00}, // '('
  {0x00, 0x60, 0x10, 0x08, 0x04, 0x04, 0x02, 0x02, 0x02, 0x02, 0x04, 0x04, 0x08, 0x10, 0x60, 0x00}, // ')'
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x5a, 0x3c, 0x3c, 0x5a, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00}, // '*'
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x7e, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00}, // '+'
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x08, 0x08, 0x00}, // ','
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // '_'
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00}, // '.'
  {0x00, 0x02, 0x02, 0x02, 0x04, 0x04, 0x04, 0x08, 0x08, 0x08, 0x10, 0x10, 0x10, 0x20, 0x20, 0x00}, // '/'
  {0x00, 0x3c, 0x24, 0x46, 0x46, 0x4a, 0x4a, 0x4a, 0x4a, 0x52, 0x52, 0x52, 0x62, 0x24, 0x3c, 0x00}, // '0'
  {0x00, 0x08, 0x18, 0x28, 0x48, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x7e, 0x00}, // '1'
  {0x00, 0x18, 0x24, 0x42, 0x02, 0x04, 0x04, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x7e, 0x00}, // '2'
  {0x00, 0x3c, 0x42, 0x42, 0x02, 0x02, 0x02, 0x1c, 0x02, 0x02, 0x02, 0x02, 0x42, 0x42, 0x3c, 0x00}, // '3'
  {0x00, 0x0c, 0x0c, 0x14, 0x14, 0x24, 0x24, 0x44, 0x44, 0x44, 0x7e, 0x04, 0x04, 0x04, 0x04, 0x00}, // '4'
  {0x00, 0x3e, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7c, 0x06, 0x02, 0x02, 0x02, 0x02, 0x02, 0x7c, 0x00}, // '5'
  {0x00, 0x3c, 0x42, 0x42, 0x40, 0x40, 0x40, 0x7c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00}, // '6'
  {0x00, 0x7e, 0x02, 0x02, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x40, 0x00}, // '7'
  {0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00}, // '8'
  {0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3e, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x7c, 0x00}, // '9'
  {0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00}, // ':'
  {0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x08, 0x08, 0x00, 0x00}, // ';'
  {0x00, 0x00, 0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x00, 0x00}, // '<'
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00}, // '='
  {0x00, 0x00, 0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00}, // '>'
  {0x00, 0x3c, 0x42, 0x42, 0x02, 0x02, 0x02, 0x1c, 0x20, 0x20, 0x20, 0x20, 0x00, 0x20, 0x20, 0x00}  // '?'
};

#endif
