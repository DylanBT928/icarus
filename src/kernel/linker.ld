ENTRY(_start)

SECTIONS
{
    /* start offset */
    . = 2M;

    /* multiboot header */
    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }

    /* read only data */
    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    /* initialized read only data */
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    /* uninitialized read only data and stack */
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }
}
