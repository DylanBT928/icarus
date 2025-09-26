# Multiboot header constants
.set ALIGN,     1<<0                # Align modules on page boundaries
.set MEMINFO,   1<<1                # Request memory map
.set FLAGS,     ALIGN | MEMINFO     # Multiboot flags
.set MAGIC,     0x1BADB002          # Multiboot magic number
.set CHECKSUM,  -(MAGIC + FLAGS)    # CHECKSUM + MAGIC + FLAGS == 0

# Multiboot header (in the first 8 KiB)
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Kernel stack (16 KiB, 16-byte aligned)
.section .bss
.align 16
stack_bottom:
    .skip 16384 # 16 KiB
stack_top:

# Entry point
.section .text
.global _start
.type _start, @function
_start:
    # CPU in 32-bit protected mode
    mov $stack_top, %esp    # Set stack pointer

    # Call C++ kernel main
    .extern kmain
    call kmain

    # Halt forever
    cli
1:  hlt
    jmp 1b

# Record size of _start symbol
.size _start, . - _start
