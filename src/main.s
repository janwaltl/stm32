.syntax unified
.thumb
.eabi_attribute Tag_ABI_align_preserved, 1

.section .text, "ax", %progbits
// Globally exported symbols
.global main

.thumb_func
main:
	// Add 1 to register 'r0'.
	ADDS r0, r0, #1
	// Loop back.
	B    main
.size main, .-main
