.syntax unified
.thumb
.eabi_attribute Tag_ABI_align_preserved, 1

.global main

.thumb_func
sleep_halfsec:
	MOVW r1, #500
	ADD r1, r8
sleep_loop:
	WFI
	MOV r0, r8
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
