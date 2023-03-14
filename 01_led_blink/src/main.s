.syntax unified
.thumb
.eabi_attribute Tag_ABI_align_preserved, 1

// Globally exported symbols
.global main

.section .text, "ax", %progbits

// Initialize green user led on F411 Nucleo board.
.thumb_func
init_green_led:
	// Enable GPIO_A peripheral clock.
	LDR r4, =0x40023830
	LDR r5, [r4]
	ORR r5, #0x1
	STR r5, [r4]

	// Set GPIO_A5 into output mode.
	LDR r4, =0x40020000
	LDR r5, [r4]
	ORR r5, #(0x01<<(2*5))
	STR r5, [r4]

	BX LR
.size init_green_led,.-init_green_led

.thumb_func
green_led_off:
	// Set GPIO_A5 disable bit
	LDR r4, =0x40020018
	MOV r5, #(0x1<<(5+16)) // Upper 16bits are for resetting GPIOs.
	STR r5, [r4]

	BX LR
.size green_led_off,.-green_led_off

.thumb_func
green_led_on:
	// Set GPIO_A5 enable bit
	LDR r4, =0x40020018
	MOV r5, #(0x1<<5)
	STR r5, [r4]

	BX LR
.size green_led_on,.-green_led_on

.thumb_func
sleep_halfsec:
	LDR r4, clock_halffreq
	LSR r4, #2 		// 4 cycles per loop
sleep_loop:
	SUBS r4, #1 	// 1 Cycle
	AND r5, r5  	// 1 Cycle
	BNE sleep_loop // 2 Cycles if taken
	BX LR
.size sleep_halfsec, . - sleep_halfsec


.type clock_halftick, %object
clock_halffreq:
.word 8000000 // 16MHz clock after reset

.thumb_func
main:
	BL init_green_led
loop:
	ADDS r0, r0, #1
	BL green_led_on
	BL sleep_halfsec
	BL green_led_off
	BL sleep_halfsec
	B loop
.size main, .-main
