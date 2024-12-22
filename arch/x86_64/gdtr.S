.code64

.extern gdtr

.global gdtr_set
.section .text

gdtr_set:
  lgdt  gdtr(%rip)
  movq  $0x8, %rax
  pushq  %rax
  leaq  .reload_cs(%rip), %rax
  pushq %rax
  lretq
.reload_cs:
  movw  $0x10, %ax
  movw  %ax, %ss
  ret
