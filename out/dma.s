	.arch armv8-a
	.file	"dma.c"
	.text
	.global	virt_dma_regs
	.bss
	.align	3
	.type	virt_dma_regs, %object
	.size	virt_dma_regs, 8
virt_dma_regs:
	.zero	8
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
