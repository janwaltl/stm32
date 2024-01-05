.syntax unified
.thumb
.eabi_attribute Tag_ABI_align_preserved, 1

.global systick_irq_handler
.global init_systick_irq

REG_SYSTICK_CTRL:  .word 0xE000E010
.type REG_SYSTICK_CTRL, %object

// TICK = 1ms
TICKS_PER_SEC: .word 16000 // 16MHz clock after reset
.type TICKS_PER_SEC, %object

// Keep number of ticks from start in R8
ticks .req r8

.thumb_func
init_systick_irq:
	// Set reload value
	LDR r1, TICKS_PER_SEC
	SUB r1, 1 // As per datasheet to get interrupt every N cycles, set it to N-1.
	LDR r0, REG_SYSTICK_CTRL
	STR r1, [r0, 4]
	// Clear current values
	MOV r1, #0
	STR r1, [r0, 8]
	MOV ticks, #0
	// Start systick
	LDR r0, =0xE000E010
	MOV r1, #0b111 // Enable interrupts, use /1 AHB clock, Start counting
	STR r1, [r0, 0]

	BX lr
.size init_systick_irq, .-init_systick_irq


.thumb_func
systick_irq_handler:
	//Tick
	ADD ticks, 1

	BX lr
	
.size systick_irq_handler, .-systick_irq_handler
