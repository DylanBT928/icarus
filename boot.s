/* constants */
.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

/* multiboot section marks this as a kernel with magic values from the multiboot spec */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* allocates a small stack bc multiboot doesn't allocate one for us */
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .text
.global _start
.type start, @function
_start:
    /* C/C++ needs a stack so we load the top of the stack into esp */
    mov $stack_top, %esp

    call kernel_main

/* Puts computer into in infinite loop once there is nothing left to do */
    cli
1:  hlt
    jmp 1b
.size _start, . - _start
