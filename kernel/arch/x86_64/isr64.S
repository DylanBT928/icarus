.intel_syntax noprefix
.section .text
.globl isr_common
.extern isr_common

.macro PUSH_ALL
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push rsi
    push rdi
    push rdx
    push rcx
    push rbx
    push rbp
.endm

.macro POP_ALL
    pop rbp
    pop rbx
    pop rcx
    pop rdx
    pop rdi
    pop rsi
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
.endm

// No CPU error code
.macro ISR_NOERR n
.globl isr_stub_\n
isr_stub_\n:
    PUSH_ALL
    mov rdi, \n
    xor rsi, rsi
    sub rsp, 8
    call isr_common
    add rsp, 8
    POP_ALL
    iretq
.endm

// CPU already pushed error code
.macro ISR_ERR n
.globl isr_stub_\n
isr_stub_\n:
    mov rsi, [rsp]
    PUSH_ALL
    mov rdi, \n
    sub rsp, 8
    call isr_common
    add rsp, 8
    POP_ALL
    add rsp, 8
    iretq
.endm

ISR_NOERR 0x00
ISR_NOERR 0x01
ISR_NOERR 0x02
ISR_NOERR 0x03
ISR_NOERR 0x04
ISR_NOERR 0x05
ISR_NOERR 0x06
ISR_NOERR 0x07
ISR_ERR 0x08
ISR_NOERR 0x09
ISR_ERR 0x0A
ISR_ERR 0x0B
ISR_ERR 0x0C
ISR_ERR 0x0D
ISR_ERR 0x0E
ISR_NOERR 0x10
ISR_ERR 0x11
ISR_NOERR 0x12
ISR_NOERR 0x13
ISR_NOERR 0x14
ISR_ERR 0x15
