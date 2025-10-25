.code64

.global isr_handler
.extern isr_handler_c
.section .text
isr_handler:
  pushq %rdi
  pushq %rsi
  pushq %rdx
  pushq %rcx
  pushq %rax
  pushq %rbx
  pushq %r15
  pushq %r14
  pushq %r13
  pushq %r12
  pushq %r11
  pushq %r10
  pushq %r9

  movq  %rsp, %rdi
  callq isr_handler_c

  popq  %r9
  popq  %r10
  popq  %r11
  popq  %r12
  popq  %r13
  popq  %r14
  popq  %r15
  popq  %rbx
  popq  %rax
  popq  %rcx
  popq  %rdx
  popq  %rsi
  popq  %rdi

  addq  $8, %rsp
  iretq

.macro ex_handler error_code
ex_handler_\error_code:
  jmp isr_handler
.endm

.macro ex_handler_no_code error_code
ex_handler_no_code_\error_code:
  pushq $\error_code
  jmp isr_handler
.endm

.macro irq_handler error_code
irq_handler_\error_code:
  pushq $\error_code
  jmp isr_handler
.endm

.macro do_nothing_handler error_code
do_nothing_handler_\error_code:
  iret
.endm

ex_handler_no_code 0
ex_handler_no_code 1
ex_handler_no_code 2
ex_handler_no_code 3
ex_handler_no_code 4
ex_handler_no_code 5
ex_handler_no_code 6
ex_handler_no_code 7
ex_handler 8
ex_handler_no_code 9
ex_handler 10
ex_handler 11
ex_handler 12
ex_handler 13
ex_handler 14
ex_handler_no_code 15
ex_handler_no_code 16
ex_handler_no_code 17
ex_handler_no_code 18
ex_handler_no_code 19
ex_handler_no_code 20
ex_handler_no_code 21
ex_handler_no_code 22
ex_handler_no_code 23
ex_handler_no_code 24
ex_handler_no_code 25
ex_handler_no_code 26
ex_handler_no_code 27
ex_handler_no_code 28
ex_handler_no_code 29
ex_handler 30
ex_handler_no_code 31
irq_handler 32
irq_handler 33
irq_handler 34

.global isr_handlers
isr_handlers:
.quad ex_handler_no_code_0
.quad ex_handler_no_code_1
.quad ex_handler_no_code_2
.quad ex_handler_no_code_3
.quad ex_handler_no_code_4
.quad ex_handler_no_code_5
.quad ex_handler_no_code_6
.quad ex_handler_no_code_7
.quad ex_handler_8
.quad ex_handler_no_code_9
.quad ex_handler_10
.quad ex_handler_11
.quad ex_handler_12
.quad ex_handler_13
.quad ex_handler_14
.quad ex_handler_no_code_15
.quad ex_handler_no_code_16
.quad ex_handler_no_code_17 
.quad ex_handler_no_code_18
.quad ex_handler_no_code_19
.quad ex_handler_no_code_20
.quad ex_handler_no_code_21
.quad ex_handler_no_code_22
.quad ex_handler_no_code_23
.quad ex_handler_no_code_24
.quad ex_handler_no_code_25
.quad ex_handler_no_code_26
.quad ex_handler_no_code_27
.quad ex_handler_no_code_28
.quad ex_handler_no_code_29
.quad ex_handler_30
.quad ex_handler_no_code_31
.quad irq_handler_32
.quad irq_handler_33
.quad irq_handler_34
