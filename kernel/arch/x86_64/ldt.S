.intel_syntax noprefix

.section .bss
    .align 8
idtr64_lim:
    .space 2
idtr64_base:
    .space 8

.section .text
.globl set_idt
.type set_idt, @function
set_idt:
    mov word ptr [idtr64_lim], di
    mov qword ptr [idtr64_base], rsi
    lidt [idtr64_lim]
    ret
