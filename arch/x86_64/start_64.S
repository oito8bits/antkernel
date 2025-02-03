.code64

.equ START_KERNEL, 0xFFFFFFFF80000000

.extern start64
.extern kmain
.global _start

.align 4096
.section .text
_start:
  cli
  callq boot_init
  movq  %rax, %rdi
  movq  $stack_end, %rsp
  leaq  kernel_table_level_4(%rip), %rax
  movq  %rax, %cr3
  movq  $higher_half_kernel, %rax
  jmp   %rax
higher_half_kernel:
  callq start64

  callq kmain
  jmp .
  ret

.section .data

.align 4096
kernel_table_level_4:
  .quad kernel_identity_table_level_3 - START_KERNEL + 0x3
  .fill 510, 8, 0
  .quad kernel_table_level_3 - START_KERNEL + 0x3

.align 4096
kernel_table_level_3:
  .fill 510, 8, 0
  .quad kernel_table_level_2 - START_KERNEL + 0x3
  .fill 1, 8, 0

.align 4096
kernel_identity_table_level_3:
  .quad kernel_table_level_2 - START_KERNEL + 0x3
  .fill 511, 8, 0

.align 4096
.global kernel_table_level_2
kernel_table_level_2:
  .set i, 0;
  .rept 512
    .quad i | 0x83
    .set i, i + (1 << 21)
  .endr

.comm stack_start, 16384
stack_end:
