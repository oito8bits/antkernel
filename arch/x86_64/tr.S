.code64

.global tr_set_tss
tr_set_tss:
  movl  $0x50, %eax
  ltr   %ax
  ret
