.syntax unified
.thumb
.eabi_attribute Tag_ABI_align_preserved, 1

// Global symbols
.global reset_irq_handler
.global _estack

// End of stack, set by linker script.
.type _estack, %object

.global halt
.thumb_func
halt:
	LDR  r0, =0xDEADC0DE
	LDR  r1, =0xDEADC0DE
	LDR  r2, =0xDEADC0DE
	LDR  r3, =0xDEADC0DE
	B halt
.size halt, .-halt

// Vector table
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

.weak default_irq_handler
.thumb_set default_irq_handler, halt

.weak systick_irq_handler
.thumb_set systick_irq_handler, default_irq_handler
