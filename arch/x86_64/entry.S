.code64

.extern syscall_table
.global entry
.section .text
entry:
  pushq %rdi
  pushq %rsi
  pushq %rdx
  pushq %rcx
  pushq %rax
  pushq %rbx
  pushq %rbp
  pushq %r15
  pushq %r14
  pushq %r13
  pushq %r12
  pushq %r11
  pushq %r10
  pushq %r9
  
  movq  %rsp, %rdi
  call syscall_handler
  
  popq  %r9
  popq  %r10
  popq  %r11
  popq  %r12
  popq  %r13
  popq  %r14
  popq  %r15
  popq  %rbp
  popq  %rbx
  popq  %rax
  popq  %rcx
  popq  %rdx
  popq  %rsi
  popq  %rdi

  sysret
