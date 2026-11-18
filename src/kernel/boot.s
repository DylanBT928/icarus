# constants
.set MAGIC, 0xE85250D6 # Multiboot2
.set ARCHITECTURE, 0 # x86
.set HEADER_LENGTH, multiboot_header_end - multiboot_header
.set CHECKSUM, -(MAGIC + ARCHITECTURE + HEADER_LENGTH)

# declare a Multiboot2 header and mark the program as a kernel
.section .multiboot, "a"
.balign 8

# See: www.gnu.org/software/grub/manual/multiboot2/multiboot.html#Header-layout
multiboot_header:

    # Multiboot2 header tag
    .long MAGIC
    .long ARCHITECTURE
    .long HEADER_LENGTH
    .long CHECKSUM

    # framebuffer tag of Multiboot2 header
    .balign 8
    .short 5  # type
    .short 0  # flags
    .long 20  # size
    .long 800 # width
    .long 600 # height
    .long 32  # depth

    # EFI boot services tag
    .balign 8
    .short 7 # type
    .short 0 # flags
    .long 8  # size

    # EFI amd64 entry address tag of Multiboot2 header
    .balign 8
    .short 9     # type
    .short 0     # flags
    .long 12     # size
    .long _start # entry addr

    # Multiboot2 end tag
    .balign 8
    .short 0
    .short 0
    .long 8

multiboot_header_end:

# explicitly allocates stack because multiboot2 does not provide one
.section .bss
.balign   16

stack_bottom:
    .skip 16384 # 16 KiB

stack_top:

.section .text
.code64
.global  _start
.type    _start, @function

_start:
    # load effective address of stack_top relative to instruction pointer 
    # and store it in stack pointer
    leaq stack_top(%rip), %rsp

    # clear direction flag so that to advance toward higher addresses of 
    # a register (System V ABI expects this to be clear)
    cld

    movl %eax, %edi
    movq %rbx, %rsi

    call kernel_main

    # if kernel_main returns (bad), fall back to hlt loop
    cli

1:
    hlt
    jmp 1b


.size _start, . - _start
