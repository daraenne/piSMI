	.arch armv8-a
	.file	"main.c"
	.text
	.section	.rodata
	.align	3
.LC0:
	.string	"init Gpio failed\r"
	.align	3
.LC1:
	.string	"init SMI et Clocks failed\r"
	.align	3
.LC2:
	.string	"setup_smi failed\r"
	.align	3
.LC3:
	.string	"fail smi_direct_write\r"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
.LFB107:
	.cfi_startproc
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	mov	x29, sp
	str	w0, [sp, 28]
	str	x1, [sp, 16]
	bl	init_gpio
	cmp	w0, 0
	bne	.L2
	adrp	x0, .LC0
	add	x0, x0, :lo12:.LC0
	bl	puts
	mov	w0, 1
	b	.L3
.L2:
	bl	init_smi_clk
	cmp	w0, 0
	bne	.L4
	adrp	x0, .LC1
	add	x0, x0, :lo12:.LC1
	bl	puts
	mov	w0, 1
	b	.L3
.L4:
	bl	setup_smi
	cmp	w0, 0
	bne	.L5
	adrp	x0, .LC2
	add	x0, x0, :lo12:.LC2
	bl	puts
	mov	w0, 1
	b	.L3
.L5:
	strb	wzr, [sp, 47]
	b	.L6
.L8:
	ldrb	w0, [sp, 47]
	mov	w2, 21
	mov	w1, w0
	mov	w0, 0
	bl	smi_direct_write
	cmp	w0, 0
	bne	.L7
	adrp	x0, .LC3
	add	x0, x0, :lo12:.LC3
	bl	puts
.L7:
	ldrb	w0, [sp, 47]
	add	w0, w0, 1
	strb	w0, [sp, 47]
.L6:
	ldrb	w0, [sp, 47]
	cmp	w0, 15
	bls	.L8
#APP
// 30 "src/main.c" 1
	nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 

// 0 "" 2
// 31 "src/main.c" 1
	nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 

// 0 "" 2
// 32 "src/main.c" 1
	nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 

// 0 "" 2
// 33 "src/main.c" 1
	nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 

// 0 "" 2
// 34 "src/main.c" 1
	nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 

// 0 "" 2
// 35 "src/main.c" 1
	nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 
nop 

// 0 "" 2
#NO_APP
	b	.L5
.L3:
	ldp	x29, x30, [sp], 48
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE107:
	.size	main, .-main
	.section	.rodata
	.align	3
.LC4:
	.string	"/dev/mem"
	.align	3
.LC5:
	.string	"Error: can't open /dev/mem, run using sudo"
	.align	3
.LC6:
	.string	"mmap Failed"
	.text
	.align	2
	.global	map_segment
	.type	map_segment, %function
map_segment:
.LFB108:
	.cfi_startproc
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	mov	x29, sp
	str	x0, [sp, 24]
	str	w1, [sp, 20]
	mov	w1, 4098
	movk	w1, 0x18, lsl 16
	adrp	x0, .LC4
	add	x0, x0, :lo12:.LC4
	bl	open
	str	w0, [sp, 44]
	ldr	w0, [sp, 44]
	cmp	w0, 0
	bge	.L10
	adrp	x0, .LC5
	add	x0, x0, :lo12:.LC5
	bl	puts
	mov	x0, 0
	b	.L11
.L10:
	ldr	w0, [sp, 20]
	ldr	x1, [sp, 24]
	uxtw	x1, w1
	mov	x5, x1
	ldr	w4, [sp, 44]
	mov	w3, 1
	mov	w2, 3
	mov	x1, x0
	mov	x0, 0
	bl	mmap
	str	x0, [sp, 32]
	ldr	x0, [sp, 32]
	cmn	x0, #1
	bne	.L12
	adrp	x0, .LC6
	add	x0, x0, :lo12:.LC6
	bl	printf
	mov	x0, 0
	b	.L11
.L12:
	ldr	w0, [sp, 44]
	bl	close
	ldr	x0, [sp, 32]
.L11:
	ldp	x29, x30, [sp], 48
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE108:
	.size	map_segment, .-map_segment
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
