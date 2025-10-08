OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)

ENTRY(kmain)

PHDRS
{
    limine_requests PT_LOAD;
    text PT_LOAD;
    rodata PT_LOAD;
    data PT_LOAD;
}

SECTIONS
{
    . = 0xFFFFFFFF80000000;

    .limine_requests : {
        KEEP(*(.limine_requests_start))
        KEEP(*(.limine_requests))
        KEEP(*(.limine_requests_end))
    } :limine_requests

    . = ALIGN(CONSTANT(MAXPAGESIZE));

    .text : {
        *(.text .text.*)
    } :text

    . = ALIGN(CONSTANT(MAXPAGESIZE));

    .rodata : {
        *(.rodata .rodata.*)
    } :rodata

    .note.gnu.build-id : {
        *(.note.gnu.build-id)
    } :rodata

    . = ALIGN(CONSTANT(MAXPAGESIZE));

    .data : {
        *(.data .data.*)
    } :data

    .init_array : {
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(.init_array .init_array.*))
        PROVIDE_HIDDEN(__init_array_end = .);
    } :data

    .fini_array : {
        PROVIDE_HIDDEN(__fini_array_start = .);
        KEEP(*(.fini_array .fini_array.*))
        PROVIDE_HIDDEN(__fini_array_end = .);
    } :data

    . = ALIGN(16);

    .bss : {
        PROVIDE(__bss_start = .);
        *(.bss .bss.*)
        *(COMMON)
        PROVIDE(__bss_end = .);
    } :data

    PROVIDE(__kernel_end = .);

    /DISCARD/ : {
        *(.eh_frame*)
        *(.eh_frame_hdr*)
        *(.gcc_except_table*)
        *(.note .note.*)
        *(.comment)
    }
}
