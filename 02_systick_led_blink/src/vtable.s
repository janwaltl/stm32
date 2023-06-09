.syntax unified
.thumb
.eabi_attribute Tag_ABI_align_preserved, 1

// Globally exported symbols
.global vtable
.global reset_irq_handler

// Vector table.
.section .vtable, "a", %progbits
.type vtable, %object
vtable:
    .word _estack 					// Initial stack pointer
    .word reset_irq_handler 		// Reset handler
	.word default_irq_handler 		// NMI
	.word default_irq_handler 		// Hard fault
	.word default_irq_handler 		// Memory fault
	.word default_irq_handler 		// Bus fault
	.word default_irq_handler 		// Usage fault
	.word 0 						// Reserved
	.word 0 						// Reserved
	.word 0 						// Reserved
	.word 0 						// Reserved
	.word default_irq_handler 		// SVCall
	.word 0 						// Reserved
	.word 0 						// Reserved
	.word default_irq_handler 		// PendSV
	.word systick_irq_handler		// Systick
	. = 0x400 						// IRQs
.size vtable, .-vtable

.section .text, "ax", %progbits

// Default interruprt handler.
.thumb_func
default_irq_handler:
	LDR  r0, =0xDEADBEEF
	LDR  r1, =0xDEADBEEF
	LDR  r2, =0xDEADBEEF
	LDR  r3, =0xDEADBEEF
	B default_irq_handler
.size default_irq_handler, .-default_irq_handler

// Set by linker script
.type _estack, %object

// Setup stack and branch to main
.thumb_func
reset_irq_handler:
	// Clean stack
	LDR  r0, =_estack
	MOV  sp, r0

	B main
.size reset_irq_handler, .-reset_irq_handler


.weak systick_irq_handler
.thumb_set systick_irq_handler, default_irq_handler
