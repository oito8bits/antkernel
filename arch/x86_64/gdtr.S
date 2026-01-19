.code64

.extern gdtr

.global gdtr_set
.section .text

gdtr_set:
  lgdt  gdtr(%rip)
  movq  $0x10, %rax
  pushq  %rax
  leaq  .reload_cs(%rip), %rax
  pushq %rax
  lretq
.reload_cs:
  movw  $0x20, %ax
  movw  %ax, %ss
  ret
