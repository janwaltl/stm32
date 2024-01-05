.syntax unified
.thumb
.eabi_attribute Tag_ABI_align_preserved, 1

.global main

// 1ms Ticks are stored in r8
ticks .req r8

.thumb_func
sleep_halfsec:
	//r1 = ticks+500
	MOVW r1, #500
	ADD r1, ticks
sleep_loop:
	// Wait until r2 reaches r1 -> 500 ticks
	WFI
	MOV r0, ticks
	CMP r0, r1
	BMI sleep_loop

	BX LR
.size sleep_halfsec, .-sleep_halfsec
	

.thumb_func
main:
	BL init_green_led
	BL init_systick_irq
loop:
	BL green_led_on
	BL sleep_halfsec
	BL green_led_off
	BL sleep_halfsec
	B loop
.size main, .-main
