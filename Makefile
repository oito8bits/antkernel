KERNEL_NAME=ant_kernel

X86-64_C_SRCS = $(wildcard arch/x86_64/*.c)
X86-64_ASM_SRCS = $(wildcard arch/x86_64/*.S)
X86-64_OBJS = $(X86-64_C_SRCS:.c=.o)
X86-64_OBJS += $(X86-64_ASM_SRCS:.S=.o)
X86-64_INCLUDE = include/arch/x86_64/

KERNEL_C_SRCS = $(wildcard kernel/*.c)
KERNEL_OBJS = $(KERNEL_C_SRCS:.c=.o)

FS_C_SRCS = $(wildcard fs/*.c fs/ramfs/*.c fs/ramfs/tar/*.c)
FS_OBJS = $(FS_C_SRCS:.c=.o)

LIBK_SRCS = $(wildcard libk/*.c)
LIBK_OBJS = $(LIBK_SRCS:.c=.o)
LIBK_INCLUDE = include/

DRIVERS_SRCS = $(shell find drivers/ -name '*.c')
DRIVERS_OBJS = $(DRIVERS_SRCS:.c=.o)
DRIVERS_INCLUDE = include/drivers/

MM_SRCS = $(wildcard mm/*.c)
MM_OBJS = $(MM_SRCS:.c=.o)

ANT_INCLUDE = include/

CC = x86_64-linux-gnu-gcc
AS = x86_64-linux-gnu-as
LD = x86_64-linux-gnu-ld
CFLAGS = -O2 -mno-sse -ffreestanding -nostdlib -I $(ANT_INCLUDE) -I $(DRIVERS_INCLUDE) -I $(X86-64_INCLUDE)
LDFLAGS = -T arch/x86_64/ant.ld -z noexecstack

$(KERNEL_NAME): $(X86-64_OBJS) $(KERNEL_OBJS) $(FS_OBJS) $(DRIVERS_OBJS) $(LIBK_OBJS) $(MM_OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

%.o: %.S
	$(AS) --64 $< -o $@

.PHONY: clean

clean:
	rm -rf $(X86-64_OBJS) $(KERNEL_OBJS) $(DRIVERS_OBJS) $(LIBK_OBJS) $(MM_OBJS) $(KERNEL_NAME)
