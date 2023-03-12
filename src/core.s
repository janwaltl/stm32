.syntax unified
.thumb
.eabi_attribute Tag_ABI_align_preserved, 1

// Global memory locations.
.global vtable
.global reset_handler

.type _estack, %object


/*
 * The actual vector table.
 * Only the size of RAM and 'reset' handler are
 * included, for simplicity.
 */
.type vtable, %object
vtable:
    .word _estack
    .word reset_handler
.size vtable, .-vtable

.thumb_func
reset_handler:
	// Set the stack pointer to the end of the stack.
	// The '_estack' value is defined in our linker script.
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
