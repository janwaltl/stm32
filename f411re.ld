_estack = 0x20001000;

ENTRY(reset_handler)

MEMORY
{
    FLASH ( rx )      : ORIGIN = 0x08000000, LENGTH = 512K
    RAM ( rxw )       : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS{
	.text : { *(.text*) } > FLASH
    .bss : { *(.bss*) } > FLASH
}
