.intel_syntax noprefix

.section .bss
    .align 8

gdtr64:
    .space 2

gdtr64_base:
    .space 8

.section .text

.globl set_gdt
.type set_gdt, @function

set_gdt:
    mov     word ptr [gdtr64], di
    mov     qword ptr [gdtr64_base], rsi
    lgdt    [gdtr64]
    ret

.globl reload_segments
.type reload_segments, @function

reload_segments:
    push    rdi
    lea     rax, [rip + .after_cs]
    push    rax
    retfq

.after_cs:
    mov     ax, si
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax
    ret
