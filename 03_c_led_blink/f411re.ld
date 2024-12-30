/* Entry point */
ENTRY(reset_irq_handler)

MEMORY
{
	/* Entry point */
    FLASH (RX)      : ORIGIN = 0x08000000, LENGTH = 512K
    RAM (RWX)       : ORIGIN = 0x20000000, LENGTH = 128K
}


_heap_min_size = 90K;
_stack_size = 32K;

_estack = ORIGIN(RAM) + LENGTH(RAM);
_bstack = _estack - _stack_size;

SECTIONS
{
	.text : 
	{ 
		/* FLASH must begin with vtable */
		KEEP (*(.vtable))
		. = ALIGN(4);
		*(.text .text.*) 
		*(.rodata .rodata.*) 
		. = ALIGN(4);
		_etext = ABSOLUTE(.);
	} > FLASH

	.init_data :
	{
		_binit_data = .;
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)))
        KEEP(*(.init_array*))
        _einit_data = .;
	} > FLASH

	/*  Keep aligned for uin32_t memcpy during loading. */
	.data : 
	{
		. = ALIGN(4);
		_bdata = ABSOLUTE(.);
		*(.data)
		LONG(0xAABBCCDD); /* Make the section non-empty, otherwise VMA is incorrect.*/
		. = ALIGN(4);
		_edata = ABSOLUTE(.);
	} >RAM  AT> FLASH

	_ldbdata = LOADADDR(.data);

	/*  Keep aligned for uin32_t memset during loading. */
    .bss (NOLOAD) : 
	{ 
		. = ALIGN(4);
		_bbss = ABSOLUTE(.);
		*(.bss*) 
		. = ALIGN(4);
		_ebss = ABSOLUTE(.);
	} > RAM

	.heap (NOLOAD) :
	{
		. = ALIGN(4);
		_bheap = ABSOLUTE(.);

		. = _bstack;
		_eheap = ABSOLUTE(.);
		_heap_size = _eheap - _bheap; 
		ASSERT(_heap_size>= _heap_min_size, "Not enough space for heap");
	} > RAM

	.stack (NOLOAD) :
	{
		. = _bstack;
		. = _estack;
	} > RAM
}



