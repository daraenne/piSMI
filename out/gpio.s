	.arch armv8-a
	.file	"gpio.c"
	.text
	.global	virt_gpio_regs
	.bss
	.align	3
	.type	virt_gpio_regs, %object
	.size	virt_gpio_regs, 8
virt_gpio_regs:
	.zero	8
	.section	.rodata
	.align	3
.LC0:
	.string	"addr gpio = %x\r\n"
	.align	3
.LC1:
	.string	"erreur lors de map_segment pour GPIO\r"
	.text
	.align	2
	.global	init_gpio
	.type	init_gpio, %function
init_gpio:
.LFB107:
	.cfi_startproc
	stp	x29, x30, [sp, -32]!
	.cfi_def_cfa_offset 32
	.cfi_offset 29, -32
	.cfi_offset 30, -24
	mov	x29, sp
	bl	bcm_host_get_peripheral_address
	str	w0, [sp, 28]
	ldr	w0, [sp, 28]
	add	w0, w0, 2097152
	mov	w1, w0
	adrp	x0, .LC0
	add	x0, x0, :lo12:.LC0
	bl	printf
	ldr	w0, [sp, 28]
	add	w0, w0, 2097152
	uxtw	x0, w0
	mov	w1, 4096
	bl	map_segment
	mov	x1, x0
	adrp	x0, virt_gpio_regs
	add	x0, x0, :lo12:virt_gpio_regs
	str	x1, [x0]
	adrp	x0, virt_gpio_regs
	add	x0, x0, :lo12:virt_gpio_regs
	ldr	x0, [x0]
	cmp	x0, 0
	bne	.L2
	adrp	x0, .LC1
	add	x0, x0, :lo12:.LC1
	bl	puts
	mov	w0, 0
	b	.L3
.L2:
	mov	w0, 1
.L3:
	ldp	x29, x30, [sp], 32
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE107:
	.size	init_gpio, .-init_gpio
	.section	.rodata
	.align	3
.LC2:
	.string	"gpio_lev_0 : 0b"
	.align	3
.LC3:
	.string	"%d"
	.align	3
.LC4:
	.string	"\r"
	.align	3
.LC5:
	.string	"gpio 18 = %d\r\n"
	.text
	.align	2
	.global	testGpio
	.type	testGpio, %function
testGpio:
.LFB108:
	.cfi_startproc
	stp	x29, x30, [sp, -32]!
	.cfi_def_cfa_offset 32
	.cfi_offset 29, -32
	.cfi_offset 30, -24
	mov	x29, sp
	strb	wzr, [sp, 31]
	b	.L5
.L6:
	adrp	x0, virt_gpio_regs
	add	x0, x0, :lo12:virt_gpio_regs
	ldr	x1, [x0]
	ldrb	w2, [sp, 31]
	mov	w0, 52429
	movk	w0, 0xcccc, lsl 16
	umull	x0, w2, w0
	lsr	x0, x0, 32
	lsr	w0, w0, 3
	and	w4, w0, 255
	and	x0, x4, 255
	lsl	x0, x0, 2
	add	x0, x1, x0
	ldr	w3, [x0]
	ldrb	w1, [sp, 31]
	mov	w0, 52429
	movk	w0, 0xcccc, lsl 16
	umull	x0, w1, w0
	lsr	x0, x0, 32
	lsr	w2, w0, 3
	mov	w0, w2
	lsl	w0, w0, 2
	add	w0, w0, w2
	lsl	w0, w0, 1
	sub	w0, w1, w0
	and	w0, w0, 255
	mov	w1, w0
	mov	w0, w1
	lsl	w0, w0, 1
	add	w0, w0, w1
	mov	w1, 7
	lsl	w0, w1, w0
	mvn	w0, w0
	mov	w2, w0
	adrp	x0, virt_gpio_regs
	add	x0, x0, :lo12:virt_gpio_regs
	ldr	x1, [x0]
	and	x0, x4, 255
	lsl	x0, x0, 2
	add	x0, x1, x0
	and	w1, w3, w2
	str	w1, [x0]
	ldrb	w0, [sp, 31]
	add	w0, w0, 1
	strb	w0, [sp, 31]
.L5:
	ldrb	w0, [sp, 31]
	cmp	w0, 15
	bls	.L6
	adrp	x0, .LC2
	add	x0, x0, :lo12:.LC2
	bl	printf
	mov	w0, 31
	strb	w0, [sp, 30]
	b	.L7
.L8:
	adrp	x0, virt_gpio_regs
	add	x0, x0, :lo12:virt_gpio_regs
	ldr	x0, [x0]
	add	x0, x0, 52
	ldr	w1, [x0]
	ldrsb	w0, [sp, 30]
	lsr	w0, w1, w0
	and	w0, w0, 1
	mov	w1, w0
	adrp	x0, .LC3
	add	x0, x0, :lo12:.LC3
	bl	printf
	ldrsb	w0, [sp, 30]
	and	w0, w0, 255
	sub	w0, w0, #1
	and	w0, w0, 255
	strb	w0, [sp, 30]
.L7:
	ldrsb	w0, [sp, 30]
	cmp	w0, 0
	bge	.L8
	adrp	x0, .LC4
	add	x0, x0, :lo12:.LC4
	bl	puts
	adrp	x0, virt_gpio_regs
	add	x0, x0, :lo12:virt_gpio_regs
	ldr	x0, [x0]
	add	x0, x0, 52
	ldr	w0, [x0]
	lsr	w0, w0, 18
	and	w0, w0, 1
	mov	w1, w0
	adrp	x0, .LC5
	add	x0, x0, :lo12:.LC5
	bl	printf
	adrp	x0, virt_gpio_regs
	add	x0, x0, :lo12:virt_gpio_regs
	ldr	x0, [x0]
	ldr	w1, [x0]
	adrp	x0, virt_gpio_regs
	add	x0, x0, :lo12:virt_gpio_regs
	ldr	x0, [x0]
	orr	w1, w1, 32768
	str	w1, [x0]
	mov	w0, 5
	strb	w0, [sp, 29]
	b	.L9
.L10:
	adrp	x0, virt_gpio_regs
	add	x0, x0, :lo12:virt_gpio_regs
	ldr	x1, [x0]
	ldrb	w2, [sp, 29]
	mov	w0, 52429
	movk	w0, 0xcccc, lsl 16
	umull	x0, w2, w0
	lsr	x0, x0, 32
	lsr	w0, w0, 3
	and	w4, w0, 255
	and	x0, x4, 255
	lsl	x0, x0, 2
	add	x0, x1, x0
	ldr	w3, [x0]
	ldrb	w1, [sp, 29]
	mov	w0, 52429
	movk	w0, 0xcccc, lsl 16
	umull	x0, w1, w0
	lsr	x0, x0, 32
	lsr	w2, w0, 3
	mov	w0, w2
	lsl	w0, w0, 2
	add	w0, w0, w2
	lsl	w0, w0, 1
	sub	w0, w1, w0
	and	w0, w0, 255
	mov	w1, w0
	mov	w0, w1
	lsl	w0, w0, 1
	add	w0, w0, w1
	mov	w1, 7
	lsl	w0, w1, w0
	mvn	w0, w0
	mov	w2, w0
	adrp	x0, virt_gpio_regs
	add	x0, x0, :lo12:virt_gpio_regs
	ldr	x1, [x0]
	and	x0, x4, 255
	lsl	x0, x0, 2
	add	x0, x1, x0
	and	w1, w3, w2
	str	w1, [x0]
	adrp	x0, virt_gpio_regs
	add	x0, x0, :lo12:virt_gpio_regs
	ldr	x1, [x0]
	ldrb	w2, [sp, 29]
	mov	w0, 52429
	movk	w0, 0xcccc, lsl 16
	umull	x0, w2, w0
	lsr	x0, x0, 32
	lsr	w0, w0, 3
	and	w4, w0, 255
	and	x0, x4, 255
	lsl	x0, x0, 2
	add	x0, x1, x0
	ldr	w3, [x0]
	ldrb	w1, [sp, 29]
	mov	w0, 52429
	movk	w0, 0xcccc, lsl 16
	umull	x0, w1, w0
	lsr	x0, x0, 32
	lsr	w2, w0, 3
	mov	w0, w2
	lsl	w0, w0, 2
	add	w0, w0, w2
	lsl	w0, w0, 1
	sub	w0, w1, w0
	and	w0, w0, 255
	mov	w1, w0
	mov	w0, w1
	lsl	w0, w0, 1
	add	w0, w0, w1
	mov	w1, 1
	lsl	w0, w1, w0
	mov	w2, w0
	adrp	x0, virt_gpio_regs
	add	x0, x0, :lo12:virt_gpio_regs
	ldr	x1, [x0]
	and	x0, x4, 255
	lsl	x0, x0, 2
	add	x0, x1, x0
	orr	w1, w3, w2
	str	w1, [x0]
	ldrb	w0, [sp, 29]
	add	w0, w0, 1
	strb	w0, [sp, 29]
.L9:
	ldrb	w0, [sp, 29]
	cmp	w0, 20
	bls	.L10
.L13:
	str	wzr, [sp, 24]
	b	.L11
.L12:
	ldr	w0, [sp, 24]
	lsl	w1, w0, 5
	adrp	x0, virt_gpio_regs
	add	x0, x0, :lo12:virt_gpio_regs
	ldr	x0, [x0]
	add	x0, x0, 40
	mvn	w1, w1
	str	w1, [x0]
	adrp	x0, virt_gpio_regs
	add	x0, x0, :lo12:virt_gpio_regs
	ldr	x0, [x0]
	add	x0, x0, 28
	ldr	w1, [sp, 24]
	lsl	w1, w1, 5
	str	w1, [x0]
	ldr	w0, [sp, 24]
	add	w0, w0, 1
	str	w0, [sp, 24]
.L11:
	ldr	w1, [sp, 24]
	mov	w0, 65534
	cmp	w1, w0
	bls	.L12
	adrp	x0, virt_gpio_regs
	add	x0, x0, :lo12:virt_gpio_regs
	ldr	x0, [x0]
	add	x0, x0, 40
	mov	w1, -1
	str	w1, [x0]
	mov	w0, 1
	bl	sleep
	b	.L13
	.cfi_endproc
.LFE108:
	.size	testGpio, .-testGpio
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
