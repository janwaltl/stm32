.syntax unified
.thumb
.eabi_attribute Tag_ABI_align_preserved, 1

// Global memory locations.
.global vtable
.global reset_handler

// Set by linker script
.type _estack, %object

// Vector table.
.type vtable, %object
vtable:
    .word _estack 					// Initial stack pointer
    .word reset_handler 			// Reset handler
	.word unhandled 				// NMI
	.word unhandled 				// Hard fault
	.word unhandled 				// Memory fault
	.word unhandled 				// Bus fault
	.word unhandled 				// Usage fault
	.word 0 						// Reserved
	.word 0 						// Reserved
	.word 0 						// Reserved
	.word 0 						// Reserved
	.word unhandled 				// SVCall
	.word 0 						// Reserved
	.word 0 						// Reserved
	.word unhandled 				// PendSV
	.word unhandled 				// Systick
	. = 0x400 						// IRQs
.size vtable, .-vtable


.thumb_func
unhandled:
	LDR  r0, =0xDEADBEEF
	LDR  r1, =0xDEADBEEF
	LDR  r2, =0xDEADBEEF
	LDR  r3, =0xDEADBEEF
	B unhandled
.size unhandled, .-unhandled

.thumb_func
reset_handler:
	// Clean stack
	LDR  r0, =_estack
	MOV  sp, r0

	// Set some dummy values. When we see these values
	// in our debugger, we'll know that our program
	// is loaded on the chip and working.
	LDR  r7, =0xDEADBEEF
	MOVS r0, #0
	B main_loop
.size reset_handler, .-reset_handler

.thumb_func
main_loop:
	// Add 1 to register 'r0'.
	ADDS r0, r0, #1
	// Loop back.
	B    main_loop
.size main_loop, .-main_loop
