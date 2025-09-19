#ifndef _INCLUDE_LAPIC_
#define _INCLUDE_LAPIC_

#define PIC_COMMAND_MASTER 0x20
#define PIC_COMMAND_SLAVE 0xA0

#define PIC_DATA_MASTER 0x21
#define PIC_DATA_SLAVE 0xA1

#define ICW_1 0x11

#define ICW_2_MASTER 0x20
#define ICW_2_SLAVE 0x28

#define ICW_3_MASTER 0x2
#define ICW_3_SLAVE 0x4

#define ICW_4 0x1

void lapic_init(void);

#endif
