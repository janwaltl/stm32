.syntax unified
.thumb
.eabi_attribute Tag_ABI_align_preserved, 1

.global systick_irq_handler
.global init_systick_irq

REG_SYSTICK_CTRL:  .word 0xE000E010
.type REG_SYSTICK_CTRL, %object

# TICK = 1MS
CONST_CLOCK_MS_TICKS: .word 16000 // 16MHz clock after reset
.type CONST_CLOCK_FREQ, %object

# Keep number of tick from start in R8

.thumb_func
init_systick_irq:
	// Clear tick counter
	MOV r8, #0
	// Set reload value
	LDR r0, REG_SYSTICK_CTRL
	LDR r1, CONST_CLOCK_MS_TICKS
	SUB r1, 1 // As per datasheet to get interrupt every N cycles, set it to N-1.
	STR r1, [r0, 4]
	// Clear current value
	MOV r1, #0
	STR r1, [r0, 8]
	// Start systick
	LDR r0, =0xE000E010
	MOV r1, #0b111 // Enable interrupts, use /1 AHB clock, Start counting
	STR r1, [r0, 0]

	BX lr
.size init_systick_irq, .-init_systick_irq


.thumb_func
systick_irq_handler:
	ADD r8, 1

	BX lr
	
.size systick_irq_handler, .-systick_irq_handler
