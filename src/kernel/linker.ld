ENTRY(_start)

SECTIONS
{
    /* start offset */
    . = 2M;

    /* multiboot header */
    .text BLOCK(4K) : ALIGN(4K)
    {
        KEEP(*(.multiboot))
        *(.text .text.*)
    }

    /* read only data */
    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata .rodata.*)
    }

    /* initialized read only data */
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data .data.*)
    }

    /* uninitialized read only data and stack */
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss .bss.*)
    }
}
