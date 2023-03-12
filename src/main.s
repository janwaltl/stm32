.syntax unified
.thumb
.eabi_attribute Tag_ABI_align_preserved, 1

.section .text, "ax", %progbits
// Globally exported symbols
.global main


.thumb_func
foo:
	ADDS r1, r1, #1
	PUSH {LR}
	BL foo
	POP {PC}
.size foo, .-foo

.thumb_func
bar:
	ADDS r2, r2, #1
	BX LR
.size bar, .-bar

.thumb_func
main:
	EOR r0, r0
	EOR r1, r1
	EOR r2, r2
loop:
	ADDS r0, r0, #1
	BL foo
	B loop
.size main, .-main
