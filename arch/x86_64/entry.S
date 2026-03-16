.code64

.extern syscall_table
.extern _end_stack
.global entry
.section .text
entry:
  swapgs

  movq %rsp, %gs:0x0
  movq %gs:0x8, %rsp
  movq 4(%rsp), %rsp

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
  pushq %r8
  
  movq  %rsp, %rdi
  call syscall_handler
  
  popq  %r8
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

  movq %gs:0x0, %rsp 

  swapgs

  sysret
