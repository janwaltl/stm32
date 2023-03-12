_estack = 0x20010000;

ENTRY(reset_irq_handler)

MEMORY
{
    FLASH ( rx )      : ORIGIN = 0x08000000, LENGTH = 512K
    RAM ( rxw )       : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
	.vtable :
	{ 
		. = ALIGN(4);
		KEEP (*(.vtable))
		. = ALIGN(4);
	} > FLASH
	.text : { *(.text*) } > FLASH
    .bss : { *(.bss*) } > FLASH
}
