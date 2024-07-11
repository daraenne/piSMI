	.arch armv8-a
	.file	"smi.c"
	.text
	.global	virt_clk_regs
	.bss
	.align	3
	.type	virt_clk_regs, %object
	.size	virt_clk_regs, 8
virt_clk_regs:
	.zero	8
	.global	virt_smi_regs
	.align	3
	.type	virt_smi_regs, %object
	.size	virt_smi_regs, 8
virt_smi_regs:
	.zero	8
	.local	smi_cs
	.comm	smi_cs,8,8
	.local	smi_l
	.comm	smi_l,8,8
	.local	smi_a
	.comm	smi_a,8,8
	.local	smi_d
	.comm	smi_d,8,8
	.local	smi_dc
	.comm	smi_dc,8,8
	.local	smi_dsr
	.comm	smi_dsr,32,8
	.local	smi_dsw
	.comm	smi_dsw,32,8
	.local	smi_dcs
	.comm	smi_dcs,8,8
	.local	smi_da
	.comm	smi_da,8,8
	.local	smi_dd
	.comm	smi_dd,8,8
	.section	.rodata
	.align	3
.LC0:
	.string	"addr smi = %x\r\n"
	.align	3
.LC1:
	.string	"erreur lors de map_segment pour SMI\r"
	.align	3
.LC2:
	.string	"addr clk = %x\r\n"
	.align	3
.LC3:
	.string	"erreur lors de map_segment pour CLK\r"
	.text
	.align	2
	.global	init_smi_clk
	.type	init_smi_clk, %function
init_smi_clk:
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
	add	w0, w0, 6291456
	mov	w1, w0
	adrp	x0, .LC0
	add	x0, x0, :lo12:.LC0
	bl	printf
	ldr	w0, [sp, 28]
	uxtw	x0, w0
	add	x0, x0, 6291456
	mov	w1, 4096
	bl	map_segment
	mov	x1, x0
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	str	x1, [x0]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	cmp	x0, 0
	bne	.L2
	adrp	x0, .LC1
	add	x0, x0, :lo12:.LC1
	bl	puts
	mov	w0, 0
	b	.L3
.L2:
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x1, [x0]
	adrp	x0, smi_cs
	add	x0, x0, :lo12:smi_cs
	str	x1, [x0]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x1, x0, 4
	adrp	x0, smi_l
	add	x0, x0, :lo12:smi_l
	str	x1, [x0]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x1, x0, 8
	adrp	x0, smi_a
	add	x0, x0, :lo12:smi_a
	str	x1, [x0]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x1, x0, 12
	adrp	x0, smi_d
	add	x0, x0, :lo12:smi_d
	str	x1, [x0]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x1, x0, 16
	adrp	x0, smi_dsr
	add	x0, x0, :lo12:smi_dsr
	str	x1, [x0]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x1, x0, 20
	adrp	x0, smi_dsw
	add	x0, x0, :lo12:smi_dsw
	str	x1, [x0]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x1, x0, 24
	adrp	x0, smi_dsr
	add	x0, x0, :lo12:smi_dsr
	str	x1, [x0, 8]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x1, x0, 28
	adrp	x0, smi_dsw
	add	x0, x0, :lo12:smi_dsw
	str	x1, [x0, 8]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x1, x0, 32
	adrp	x0, smi_dsr
	add	x0, x0, :lo12:smi_dsr
	str	x1, [x0, 16]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x1, x0, 36
	adrp	x0, smi_dsw
	add	x0, x0, :lo12:smi_dsw
	str	x1, [x0, 16]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x1, x0, 40
	adrp	x0, smi_dsr
	add	x0, x0, :lo12:smi_dsr
	str	x1, [x0, 24]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x1, x0, 44
	adrp	x0, smi_dsw
	add	x0, x0, :lo12:smi_dsw
	str	x1, [x0, 24]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x1, x0, 48
	adrp	x0, smi_dc
	add	x0, x0, :lo12:smi_dc
	str	x1, [x0]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x1, x0, 52
	adrp	x0, smi_dcs
	add	x0, x0, :lo12:smi_dcs
	str	x1, [x0]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x1, x0, 56
	adrp	x0, smi_da
	add	x0, x0, :lo12:smi_da
	str	x1, [x0]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x1, x0, 60
	adrp	x0, smi_dd
	add	x0, x0, :lo12:smi_dd
	str	x1, [x0]
	ldr	w0, [sp, 28]
	add	w0, w0, 1052672
	mov	w1, w0
	adrp	x0, .LC2
	add	x0, x0, :lo12:.LC2
	bl	printf
	ldr	w0, [sp, 28]
	uxtw	x0, w0
	add	x0, x0, 1052672
	mov	w1, 4096
	bl	map_segment
	mov	x1, x0
	adrp	x0, virt_clk_regs
	add	x0, x0, :lo12:virt_clk_regs
	str	x1, [x0]
	adrp	x0, virt_clk_regs
	add	x0, x0, :lo12:virt_clk_regs
	ldr	x0, [x0]
	cmp	x0, 0
	bne	.L4
	adrp	x0, .LC3
	add	x0, x0, :lo12:.LC3
	bl	puts
	mov	w0, 0
	b	.L3
.L4:
	mov	w0, 1
.L3:
	ldp	x29, x30, [sp], 32
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE107:
	.size	init_smi_clk, .-init_smi_clk
	.section	.rodata
	.align	3
.LC4:
	.string	"init SMI et Clocks failed\r"
	.align	3
.LC5:
	.string	"init Gpio failed\r"
	.text
	.align	2
	.global	setup_smi
	.type	setup_smi, %function
setup_smi:
.LFB108:
	.cfi_startproc
	stp	x29, x30, [sp, -32]!
	.cfi_def_cfa_offset 32
	.cfi_offset 29, -32
	.cfi_offset 30, -24
	mov	x29, sp
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	cmp	x0, 0
	beq	.L6
	adrp	x0, virt_clk_regs
	add	x0, x0, :lo12:virt_clk_regs
	ldr	x0, [x0]
	cmp	x0, 0
	bne	.L7
.L6:
	bl	init_smi_clk
	cmp	w0, 0
	bne	.L7
	adrp	x0, .LC4
	add	x0, x0, :lo12:.LC4
	bl	puts
	mov	w0, 0
	b	.L8
.L7:
	adrp	x0, :got:virt_gpio_regs
	ldr	x0, [x0, :got_lo12:virt_gpio_regs]
	ldr	x0, [x0]
	cmp	x0, 0
	bne	.L9
	bl	init_gpio
	cmp	w0, 0
	bne	.L9
	adrp	x0, .LC5
	add	x0, x0, :lo12:.LC5
	bl	puts
	mov	w0, 0
	b	.L8
.L9:
	strb	wzr, [sp, 31]
	b	.L10
.L11:
	adrp	x0, :got:virt_gpio_regs
	ldr	x0, [x0, :got_lo12:virt_gpio_regs]
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
	adrp	x0, :got:virt_gpio_regs
	ldr	x0, [x0, :got_lo12:virt_gpio_regs]
	ldr	x1, [x0]
	and	x0, x4, 255
	lsl	x0, x0, 2
	add	x0, x1, x0
	and	w1, w3, w2
	str	w1, [x0]
	adrp	x0, :got:virt_gpio_regs
	ldr	x0, [x0, :got_lo12:virt_gpio_regs]
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
	mov	w1, 5
	lsl	w0, w1, w0
	mov	w2, w0
	adrp	x0, :got:virt_gpio_regs
	ldr	x0, [x0, :got_lo12:virt_gpio_regs]
	ldr	x1, [x0]
	and	x0, x4, 255
	lsl	x0, x0, 2
	add	x0, x1, x0
	orr	w1, w3, w2
	str	w1, [x0]
	ldrb	w0, [sp, 31]
	add	w0, w0, 1
	strb	w0, [sp, 31]
.L10:
	ldrb	w0, [sp, 31]
	cmp	w0, 25
	bls	.L11
	adrp	x0, virt_clk_regs
	add	x0, x0, :lo12:virt_clk_regs
	ldr	x0, [x0]
	add	x0, x0, 176
	str	x0, [sp, 16]
	ldr	x0, [sp, 16]
	mov	w1, 1509949440
	str	w1, [x0]
	ldr	x0, [sp, 16]
	add	x0, x0, 4
	mov	w1, 16384
	movk	w1, 0x5a00, lsl 16
	str	w1, [x0]
	ldr	x0, [sp, 16]
	mov	w1, 22
	movk	w1, 0x5a00, lsl 16
	str	w1, [x0]
	strb	wzr, [sp, 30]
	b	.L12
.L13:
	ldrb	w1, [sp, 30]
	adrp	x0, smi_dsr
	add	x0, x0, :lo12:smi_dsr
	sxtw	x1, w1
	ldr	x1, [x0, x1, lsl 3]
	ldr	w0, [x1]
	mov	w2, 1
	bfi	w0, w2, 30, 2
	str	w0, [x1]
	ldrb	w1, [sp, 30]
	adrp	x0, smi_dsr
	add	x0, x0, :lo12:smi_dsr
	sxtw	x1, w1
	ldr	x1, [x0, x1, lsl 3]
	ldr	w0, [x1]
	mov	w2, 9
	bfi	w0, w2, 24, 6
	str	w0, [x1]
	ldrb	w1, [sp, 30]
	adrp	x0, smi_dsr
	add	x0, x0, :lo12:smi_dsr
	sxtw	x1, w1
	ldr	x1, [x0, x1, lsl 3]
	ldr	w0, [x1]
	and	w0, w0, -8388609
	str	w0, [x1]
	ldrb	w1, [sp, 30]
	adrp	x0, smi_dsr
	add	x0, x0, :lo12:smi_dsr
	sxtw	x1, w1
	ldr	x1, [x0, x1, lsl 3]
	ldr	w0, [x1]
	mov	w2, 45
	bfi	w0, w2, 16, 6
	str	w0, [x1]
	ldrb	w1, [sp, 30]
	adrp	x0, smi_dsr
	add	x0, x0, :lo12:smi_dsr
	sxtw	x1, w1
	ldr	x1, [x0, x1, lsl 3]
	ldr	w0, [x1]
	orr	w0, w0, 32768
	str	w0, [x1]
	ldrb	w1, [sp, 30]
	adrp	x0, smi_dsr
	add	x0, x0, :lo12:smi_dsr
	sxtw	x1, w1
	ldr	x1, [x0, x1, lsl 3]
	ldr	w0, [x1]
	mov	w2, 1
	bfi	w0, w2, 8, 7
	str	w0, [x1]
	ldrb	w1, [sp, 30]
	adrp	x0, smi_dsr
	add	x0, x0, :lo12:smi_dsr
	sxtw	x1, w1
	ldr	x1, [x0, x1, lsl 3]
	ldr	w0, [x1]
	mov	w2, 21
	bfi	w0, w2, 0, 7
	str	w0, [x1]
	ldrb	w1, [sp, 30]
	adrp	x0, smi_dsw
	add	x0, x0, :lo12:smi_dsw
	sxtw	x1, w1
	ldr	x1, [x0, x1, lsl 3]
	ldr	w0, [x1]
	mov	w2, 1
	bfi	w0, w2, 30, 2
	str	w0, [x1]
	ldrb	w1, [sp, 30]
	adrp	x0, smi_dsw
	add	x0, x0, :lo12:smi_dsw
	sxtw	x1, w1
	ldr	x1, [x0, x1, lsl 3]
	ldr	w0, [x1]
	mov	w2, 9
	bfi	w0, w2, 24, 6
	str	w0, [x1]
	ldrb	w1, [sp, 30]
	adrp	x0, smi_dsw
	add	x0, x0, :lo12:smi_dsw
	sxtw	x1, w1
	ldr	x1, [x0, x1, lsl 3]
	ldr	w0, [x1]
	and	w0, w0, -8388609
	str	w0, [x1]
	ldrb	w1, [sp, 30]
	adrp	x0, smi_dsw
	add	x0, x0, :lo12:smi_dsw
	sxtw	x1, w1
	ldr	x1, [x0, x1, lsl 3]
	ldr	w0, [x1]
	mov	w2, 45
	bfi	w0, w2, 16, 6
	str	w0, [x1]
	ldrb	w1, [sp, 30]
	adrp	x0, smi_dsw
	add	x0, x0, :lo12:smi_dsw
	sxtw	x1, w1
	ldr	x1, [x0, x1, lsl 3]
	ldr	w0, [x1]
	orr	w0, w0, 32768
	str	w0, [x1]
	ldrb	w1, [sp, 30]
	adrp	x0, smi_dsw
	add	x0, x0, :lo12:smi_dsw
	sxtw	x1, w1
	ldr	x1, [x0, x1, lsl 3]
	ldr	w0, [x1]
	mov	w2, 1
	bfi	w0, w2, 8, 7
	str	w0, [x1]
	ldrb	w1, [sp, 30]
	adrp	x0, smi_dsw
	add	x0, x0, :lo12:smi_dsw
	sxtw	x1, w1
	ldr	x1, [x0, x1, lsl 3]
	ldr	w0, [x1]
	mov	w2, 21
	bfi	w0, w2, 0, 7
	str	w0, [x1]
	ldrb	w0, [sp, 30]
	add	w0, w0, 1
	strb	w0, [sp, 30]
.L12:
	ldrb	w0, [sp, 30]
	cmp	w0, 3
	bls	.L13
	adrp	x0, smi_dsr
	add	x0, x0, :lo12:smi_dsr
	ldr	x1, [x0, 8]
	ldr	w0, [x1]
	orr	w0, w0, 128
	str	w0, [x1]
	adrp	x0, smi_dsw
	add	x0, x0, :lo12:smi_dsw
	ldr	x1, [x0, 8]
	ldr	w0, [x1]
	orr	w0, w0, 128
	str	w0, [x1]
	adrp	x0, smi_dc
	add	x0, x0, :lo12:smi_dc
	ldr	x1, [x0]
	ldr	w0, [x1]
	orr	w0, w0, 16777216
	str	w0, [x1]
	mov	w0, 1
.L8:
	ldp	x29, x30, [sp], 32
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE108:
	.size	setup_smi, .-setup_smi
	.align	2
	.global	set_smi_timing
	.type	set_smi_timing, %function
set_smi_timing:
.LFB109:
	.cfi_startproc
	sub	sp, sp, #32
	.cfi_def_cfa_offset 32
	str	w0, [sp, 28]
	str	w1, [sp, 24]
	str	w2, [sp, 20]
	str	w3, [sp, 16]
	str	w4, [sp, 12]
	str	w5, [sp, 8]
	ldr	w0, [sp, 24]
	cmp	w0, 0
	beq	.L15
	adrp	x0, smi_dsr
	add	x0, x0, :lo12:smi_dsr
	ldr	w1, [sp, 28]
	ldr	x1, [x0, x1, lsl 3]
	ldr	w0, [sp, 20]
	and	w0, w0, 63
	and	w2, w0, 255
	ldr	w0, [x1]
	bfi	w0, w2, 24, 6
	str	w0, [x1]
	adrp	x0, smi_dsr
	add	x0, x0, :lo12:smi_dsr
	ldr	w1, [sp, 28]
	ldr	x1, [x0, x1, lsl 3]
	ldr	w0, [sp, 16]
	and	w0, w0, 127
	and	w2, w0, 255
	ldr	w0, [x1]
	bfi	w0, w2, 0, 7
	str	w0, [x1]
	adrp	x0, smi_dsr
	add	x0, x0, :lo12:smi_dsr
	ldr	w1, [sp, 28]
	ldr	x1, [x0, x1, lsl 3]
	ldr	w0, [sp, 12]
	and	w0, w0, 63
	and	w2, w0, 255
	ldr	w0, [x1]
	bfi	w0, w2, 16, 6
	str	w0, [x1]
	adrp	x0, smi_dsr
	add	x0, x0, :lo12:smi_dsr
	ldr	w1, [sp, 28]
	ldr	x1, [x0, x1, lsl 3]
	ldr	w0, [sp, 8]
	and	w0, w0, 127
	and	w2, w0, 255
	ldr	w0, [x1]
	bfi	w0, w2, 8, 7
	str	w0, [x1]
	b	.L16
.L15:
	adrp	x0, smi_dsw
	add	x0, x0, :lo12:smi_dsw
	ldr	w1, [sp, 28]
	ldr	x1, [x0, x1, lsl 3]
	ldr	w0, [sp, 20]
	and	w0, w0, 63
	and	w2, w0, 255
	ldr	w0, [x1]
	bfi	w0, w2, 24, 6
	str	w0, [x1]
	adrp	x0, smi_dsw
	add	x0, x0, :lo12:smi_dsw
	ldr	w1, [sp, 28]
	ldr	x1, [x0, x1, lsl 3]
	ldr	w0, [sp, 16]
	and	w0, w0, 127
	and	w2, w0, 255
	ldr	w0, [x1]
	bfi	w0, w2, 0, 7
	str	w0, [x1]
	adrp	x0, smi_dsw
	add	x0, x0, :lo12:smi_dsw
	ldr	w1, [sp, 28]
	ldr	x1, [x0, x1, lsl 3]
	ldr	w0, [sp, 12]
	and	w0, w0, 63
	and	w2, w0, 255
	ldr	w0, [x1]
	bfi	w0, w2, 16, 6
	str	w0, [x1]
	adrp	x0, smi_dsw
	add	x0, x0, :lo12:smi_dsw
	ldr	w1, [sp, 28]
	ldr	x1, [x0, x1, lsl 3]
	ldr	w0, [sp, 8]
	and	w0, w0, 127
	and	w2, w0, 255
	ldr	w0, [x1]
	bfi	w0, w2, 8, 7
	str	w0, [x1]
.L16:
	nop
	add	sp, sp, 32
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE109:
	.size	set_smi_timing, .-set_smi_timing
	.align	2
	.global	smi_set_pio_timing
	.type	smi_set_pio_timing, %function
smi_set_pio_timing:
.LFB110:
	.cfi_startproc
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	str	w0, [sp, 12]
	nop
	add	sp, sp, 16
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE110:
	.size	smi_set_pio_timing, .-smi_set_pio_timing
	.align	2
	.global	smi_direct_write
	.type	smi_direct_write, %function
smi_direct_write:
.LFB111:
	.cfi_startproc
	sub	sp, sp, #32
	.cfi_def_cfa_offset 32
	str	w0, [sp, 12]
	str	w1, [sp, 8]
	str	w2, [sp, 4]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x0, x0, 52
	ldr	w0, [x0]
	str	w0, [sp, 24]
	ldr	w0, [sp, 24]
	and	w0, w0, 4
	cmp	w0, 0
	beq	.L19
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x0, x0, 52
	mov	w1, 4
	str	w1, [x0]
.L19:
	ldr	w0, [sp, 12]
	lsl	w0, w0, 8
	and	w2, w0, 768
	ldr	w0, [sp, 4]
	and	w1, w0, 63
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x0, x0, 56
	orr	w1, w2, w1
	str	w1, [x0]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x0, x0, 60
	ldr	w1, [sp, 8]
	str	w1, [x0]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x0, x0, 52
	mov	w1, 11
	str	w1, [x0]
	mov	w0, 50
	str	w0, [sp, 28]
.L21:
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x0, x0, 52
	ldr	w0, [x0]
	str	w0, [sp, 24]
	ldr	w0, [sp, 28]
	sub	w0, w0, #1
	str	w0, [sp, 28]
	ldr	w0, [sp, 28]
	cmp	w0, 0
	beq	.L20
	ldr	w0, [sp, 24]
	and	w0, w0, 4
	cmp	w0, 0
	beq	.L21
.L20:
	ldr	w0, [sp, 28]
	cmp	w0, 0
	bne	.L22
	mov	w0, 0
	b	.L23
.L22:
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x0, x0, 52
	mov	w1, 4
	str	w1, [x0]
	mov	w0, 1
.L23:
	add	sp, sp, 32
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE111:
	.size	smi_direct_write, .-smi_direct_write
	.align	2
	.global	smi_direct_read
	.type	smi_direct_read, %function
smi_direct_read:
.LFB112:
	.cfi_startproc
	sub	sp, sp, #32
	.cfi_def_cfa_offset 32
	str	w0, [sp, 12]
	str	x1, [sp]
	str	w2, [sp, 8]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x0, x0, 52
	ldr	w0, [x0]
	str	w0, [sp, 24]
	ldr	w0, [sp, 24]
	and	w0, w0, 4
	cmp	w0, 0
	beq	.L25
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x0, x0, 52
	mov	w1, 4
	str	w1, [x0]
.L25:
	ldr	w0, [sp, 12]
	lsl	w0, w0, 8
	and	w2, w0, 768
	ldr	w0, [sp, 8]
	and	w1, w0, 63
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x0, x0, 56
	orr	w1, w2, w1
	str	w1, [x0]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x0, x0, 52
	mov	w1, 3
	str	w1, [x0]
	mov	w0, 50
	str	w0, [sp, 28]
.L27:
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x0, x0, 52
	ldr	w0, [x0]
	str	w0, [sp, 24]
	ldr	w0, [sp, 28]
	sub	w0, w0, #1
	str	w0, [sp, 28]
	ldr	w0, [sp, 28]
	cmp	w0, 0
	beq	.L26
	ldr	w0, [sp, 24]
	and	w0, w0, 4
	cmp	w0, 0
	beq	.L27
.L26:
	ldr	w0, [sp, 28]
	cmp	w0, 0
	bne	.L28
	mov	w0, 0
	b	.L29
.L28:
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x0, x0, 60
	ldr	w1, [x0]
	ldr	x0, [sp]
	str	w1, [x0]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x0, x0, 52
	mov	w1, 4
	str	w1, [x0]
	mov	w0, 1
.L29:
	add	sp, sp, 32
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE112:
	.size	smi_direct_read, .-smi_direct_read
	.align	2
	.global	smi_block_write
	.type	smi_block_write, %function
smi_block_write:
.LFB113:
	.cfi_startproc
	stp	x19, x20, [sp, -64]!
	.cfi_def_cfa_offset 64
	.cfi_offset 19, -64
	.cfi_offset 20, -56
	str	w0, [sp, 44]
	str	w1, [sp, 40]
	str	x2, [sp, 32]
	str	w3, [sp, 28]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x19, [x0]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x20, x0, 12
	mov	w0, 16
	str	w0, [x19]
	mov	w0, 35
	str	w0, [x19]
	ldr	w0, [sp, 44]
	lsl	w0, w0, 8
	and	w2, w0, 768
	ldr	w0, [sp, 28]
	and	w1, w0, 63
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x0, x0, 8
	orr	w1, w2, w1
	str	w1, [x0]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x0, x0, 4
	ldr	w1, [sp, 40]
	str	w1, [x0]
	ldr	w0, [x19]
	str	w0, [sp, 56]
	b	.L31
.L33:
	ldr	x0, [sp, 32]
	add	x1, x0, 2
	str	x1, [sp, 32]
	ldrsh	w0, [x0]
	str	w0, [x20]
	ldr	w0, [sp, 40]
	sub	w0, w0, #1
	str	w0, [sp, 40]
	ldr	w0, [x19]
	str	w0, [sp, 56]
.L31:
	ldr	w0, [sp, 40]
	cmp	w0, 0
	beq	.L32
	ldr	w0, [sp, 56]
	and	w0, w0, 268435456
	cmp	w0, 0
	bne	.L33
.L32:
	mov	w0, 41
	str	w0, [x19]
	mov	w0, 500
	str	w0, [sp, 60]
	b	.L34
.L36:
	ldr	w0, [x19]
	str	w0, [sp, 56]
	ldr	w0, [sp, 56]
	and	w0, w0, 268435456
	cmp	w0, 0
	beq	.L34
	ldr	x0, [sp, 32]
	add	x1, x0, 2
	str	x1, [sp, 32]
	ldrsh	w0, [x0]
	str	w0, [x20]
	ldr	w0, [sp, 40]
	sub	w0, w0, #1
	str	w0, [sp, 40]
	mov	w0, 500
	str	w0, [sp, 60]
.L34:
	ldr	w0, [sp, 60]
	sub	w0, w0, #1
	str	w0, [sp, 60]
	ldr	w0, [sp, 60]
	cmp	w0, 0
	beq	.L35
	ldr	w0, [sp, 40]
	cmp	w0, 0
	bne	.L36
.L35:
	ldr	w0, [sp, 60]
	cmp	w0, 0
	bne	.L37
	mov	w0, 0
	b	.L38
.L37:
	mov	w0, 500
	str	w0, [sp, 60]
.L40:
	ldr	w0, [x19]
	str	w0, [sp, 56]
	ldr	w0, [sp, 60]
	sub	w0, w0, #1
	str	w0, [sp, 60]
	ldr	w0, [sp, 60]
	cmp	w0, 0
	beq	.L39
	ldr	w0, [sp, 56]
	and	w0, w0, 4
	cmp	w0, 0
	bne	.L40
.L39:
	ldr	w0, [sp, 60]
	cmp	w0, 0
	bne	.L41
	mov	w0, 0
	b	.L38
.L41:
	mov	w0, 3
	str	w0, [x19]
	mov	w0, 1
.L38:
	ldp	x19, x20, [sp], 64
	.cfi_restore 20
	.cfi_restore 19
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE113:
	.size	smi_block_write, .-smi_block_write
	.section	.rodata
	.align	3
.LC6:
	.string	"%d\n"
	.text
	.align	2
	.global	smi_block_read
	.type	smi_block_read, %function
smi_block_read:
.LFB114:
	.cfi_startproc
	stp	x29, x30, [sp, -80]!
	.cfi_def_cfa_offset 80
	.cfi_offset 29, -80
	.cfi_offset 30, -72
	mov	x29, sp
	stp	x19, x20, [sp, 16]
	.cfi_offset 19, -64
	.cfi_offset 20, -56
	str	w0, [sp, 60]
	str	w1, [sp, 56]
	str	x2, [sp, 48]
	str	w3, [sp, 44]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x19, [x0]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x20, x0, 12
	mov	w0, 19
	str	w0, [x19]
	ldr	w0, [sp, 60]
	lsl	w0, w0, 8
	and	w2, w0, 768
	ldr	w0, [sp, 44]
	and	w1, w0, 63
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x0, x0, 8
	orr	w1, w2, w1
	str	w1, [x0]
	adrp	x0, virt_smi_regs
	add	x0, x0, :lo12:virt_smi_regs
	ldr	x0, [x0]
	add	x0, x0, 4
	ldr	w1, [sp, 56]
	str	w1, [x0]
	mov	w0, 9
	str	w0, [x19]
	b	.L43
.L45:
	ldr	w2, [x20]
	ldr	x0, [sp, 48]
	add	x1, x0, 2
	str	x1, [sp, 48]
	sxth	w1, w2
	strh	w1, [x0]
	ldr	w0, [sp, 56]
	sub	w0, w0, #1
	str	w0, [sp, 56]
.L44:
	ldr	w0, [x19]
	and	w0, w0, 536870912
	cmp	w0, 0
	bne	.L45
	ldr	w1, [sp, 56]
	adrp	x0, .LC6
	add	x0, x0, :lo12:.LC6
	bl	printf
.L43:
	ldr	w0, [sp, 56]
	cmp	w0, 0
	bne	.L44
	mov	w0, 500
	str	w0, [sp, 76]
.L48:
	ldr	w0, [x19]
	str	w0, [sp, 72]
	ldr	w0, [sp, 76]
	sub	w0, w0, #1
	str	w0, [sp, 76]
	ldr	w0, [sp, 76]
	cmp	w0, 0
	beq	.L47
	ldr	w0, [sp, 72]
	and	w0, w0, 4
	cmp	w0, 0
	bne	.L48
.L47:
	ldr	w0, [sp, 76]
	cmp	w0, 0
	bne	.L49
	mov	w0, 0
	b	.L50
.L49:
	mov	w0, 3
	str	w0, [x19]
	mov	w0, 1
.L50:
	ldp	x19, x20, [sp, 16]
	ldp	x29, x30, [sp], 80
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE114:
	.size	smi_block_read, .-smi_block_read
	.section	.rodata
	.align	3
	.type	pio_timing, %object
	.size	pio_timing, 80
pio_timing:
	.word	70
	.word	165
	.word	5
	.word	600
	.word	50
	.word	125
	.word	5
	.word	383
	.word	30
	.word	100
	.word	5
	.word	240
	.word	30
	.word	80
	.word	5
	.word	180
	.word	25
	.word	70
	.word	5
	.word	120
	.text
	.align	2
	.global	set_pio_timing
	.type	set_pio_timing, %function
set_pio_timing:
.LFB115:
	.cfi_startproc
	stp	x29, x30, [sp, -80]!
	.cfi_def_cfa_offset 80
	.cfi_offset 29, -80
	.cfi_offset 30, -72
	mov	x29, sp
	str	w0, [sp, 28]
	ldr	w0, [sp, 28]
	cmp	w0, 0
	blt	.L52
	ldr	w0, [sp, 28]
	cmp	w0, 4
	ble	.L53
.L52:
	mov	w0, 0
	b	.L54
.L53:
	mov	w0, 22848
	movk	w0, 0x773, lsl 16
	str	w0, [sp, 64]
	ldr	w0, [sp, 64]
	ucvtf	d0, w0
	mov	x0, 225833675390976
	movk	x0, 0x41cd, lsl 48
	fmov	d1, x0
	fdiv	d0, d1, d0
	str	d0, [sp, 56]
	adrp	x0, pio_timing
	add	x1, x0, :lo12:pio_timing
	ldrsw	x0, [sp, 28]
	lsl	x0, x0, 4
	add	x0, x1, x0
	ldr	w0, [x0]
	scvtf	d1, w0
	ldr	d0, [sp, 56]
	fdiv	d0, d1, d0
	adrp	x0, .LC7
	ldr	d1, [x0, #:lo12:.LC7]
	fadd	d0, d0, d1
	fcvtzs	w0, d0
	str	w0, [sp, 76]
	ldr	w0, [sp, 76]
	cmp	w0, 0
	bne	.L55
	mov	w0, 1
	str	w0, [sp, 76]
.L55:
	ldr	w0, [sp, 76]
	cmp	w0, 64
	bne	.L56
	str	wzr, [sp, 76]
.L56:
	ldr	w0, [sp, 76]
	cmp	w0, 64
	ble	.L57
	mov	w0, 0
	b	.L54
.L57:
	adrp	x0, pio_timing
	add	x1, x0, :lo12:pio_timing
	ldrsw	x0, [sp, 28]
	lsl	x0, x0, 4
	add	x0, x1, x0
	ldr	w0, [x0, 4]
	scvtf	d1, w0
	ldr	d0, [sp, 56]
	fdiv	d0, d1, d0
	adrp	x0, .LC7
	ldr	d1, [x0, #:lo12:.LC7]
	fadd	d0, d0, d1
	fcvtzs	w0, d0
	str	w0, [sp, 72]
	ldr	w0, [sp, 72]
	cmp	w0, 0
	bne	.L58
	mov	w0, 1
	str	w0, [sp, 72]
.L58:
	ldr	w0, [sp, 72]
	cmp	w0, 128
	bne	.L59
	str	wzr, [sp, 72]
.L59:
	ldr	w0, [sp, 72]
	cmp	w0, 128
	ble	.L60
	mov	w0, 0
	b	.L54
.L60:
	ldr	w1, [sp, 76]
	ldr	w0, [sp, 72]
	add	w0, w1, w0
	scvtf	d0, w0
	ldr	d1, [sp, 56]
	fmul	d0, d1, d0
	str	d0, [sp, 48]
	adrp	x0, pio_timing
	add	x1, x0, :lo12:pio_timing
	ldrsw	x0, [sp, 28]
	lsl	x0, x0, 4
	add	x0, x1, x0
	ldr	w0, [x0, 12]
	scvtf	d1, w0
	ldr	d0, [sp, 48]
	fsub	d1, d1, d0
	ldr	d0, [sp, 56]
	fdiv	d0, d1, d0
	adrp	x0, .LC7
	ldr	d1, [x0, #:lo12:.LC7]
	fadd	d0, d0, d1
	fcvtzs	w0, d0
	str	w0, [sp, 68]
	ldr	w0, [sp, 68]
	cmp	w0, 0
	bne	.L61
	mov	w0, 1
	str	w0, [sp, 68]
.L61:
	ldr	w0, [sp, 68]
	cmp	w0, 64
	bne	.L62
	str	wzr, [sp, 68]
.L62:
	ldr	w0, [sp, 68]
	cmp	w0, 64
	ble	.L63
	mov	w0, 0
	b	.L54
.L63:
	mov	w0, 1
	str	w0, [sp, 44]
	ldr	w0, [sp, 76]
	ldr	w1, [sp, 72]
	ldr	w2, [sp, 68]
	ldr	w3, [sp, 44]
	mov	w5, w3
	mov	w4, w2
	mov	w3, w1
	mov	w2, w0
	mov	w1, 1
	mov	w0, 0
	bl	set_smi_timing
	ldr	w0, [sp, 76]
	ldr	w1, [sp, 72]
	ldr	w2, [sp, 68]
	ldr	w3, [sp, 44]
	mov	w5, w3
	mov	w4, w2
	mov	w3, w1
	mov	w2, w0
	mov	w1, 0
	mov	w0, 0
	bl	set_smi_timing
	ldr	w0, [sp, 76]
	ldr	w1, [sp, 72]
	ldr	w2, [sp, 68]
	ldr	w3, [sp, 44]
	mov	w5, w3
	mov	w4, w2
	mov	w3, w1
	mov	w2, w0
	mov	w1, 1
	mov	w0, 1
	bl	set_smi_timing
	ldr	w0, [sp, 76]
	ldr	w1, [sp, 72]
	ldr	w2, [sp, 68]
	ldr	w3, [sp, 44]
	mov	w5, w3
	mov	w4, w2
	mov	w3, w1
	mov	w2, w0
	mov	w1, 0
	mov	w0, 1
	bl	set_smi_timing
	mov	w0, 1
.L54:
	ldp	x29, x30, [sp], 80
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE115:
	.size	set_pio_timing, .-set_pio_timing
	.section	.rodata
	.align	3
.LC7:
	.word	-858993459
	.word	1072483532
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
