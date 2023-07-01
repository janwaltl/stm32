/* 128KB ram, stack is at top. */
_estack = 0x20020000;

/* Reserve lower 64KB for heap. */
_eheap = 0x20010000;
_bheap = 0x20000000;

ENTRY(reset_irq_handler)

MEMORY
{
    FLASH ( rx )      : ORIGIN = 0x08000000, LENGTH = 512K
    RAM ( rxw )       : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
	.text : 
	{ 
		KEEP (*(.vtable))
		. = ALIGN(4);
		*(.text .text.*) 
		*(.rodata .rodata.*) 
		. = ALIGN(4);
	} > FLASH
    .bss : 
	{ 
		*(.bss*) 
	} > FLASH
}
