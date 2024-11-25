#ifndef DMA_H
#define DMA_H

#include <linux/io.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>

#define __debug 0

#define PERIPH_BASE		0xFE000000
#define BUS_BASE		0x7E000000
#define DMA_BASE		0x007000
#define DMA_chOffset	0x100		// sauf pour chan 15 qui est a : DMA_BASE + 0xE05000
#define DMA_ENOffsett	0xff0
#define DMA_REGS_SIZE	0x1000

#define DMA_CHAN		5
#define CB_CNT			2
#define BUS_TO_PHYS(x) 	((x) & ~0xC0000000)

#define DMA_CS_FIELDS active:1, end:1, interrupt:1, dreq:1, paused:1, dreq_stop_dma:1, waiting_for_outstanding_writes:1, _x1:1,\
						error:1, _x2:7, priority:4, panic_priority:4, _x3:4, wait_for_outstanding_writes:1, disdebug:1, abort:1, reset:1
typedef union{
	struct{ volatile uint32_t DMA_CS_FIELDS; };
	volatile uint32_t reg;
}DMA_CS_T;

#define DMA_TX_INFO_FIELDS inten:1, tdmode:1, _x1:1, wait_resp:1, dest_inc:1, dest_width:1, dest_dreq:1, dest_ignore:1,\
							src_inc:1, src_width:1,src_dreq:1, src_ignore:1, burst_len:4,permap:5, wait_cyc:5, no_wide_burst:1
typedef union{
	struct{ volatile uint32_t DMA_TX_INFO_FIELDS; };
	volatile uint32_t reg;
}DMA_TX_INFO_T;

typedef struct{
    DMA_TX_INFO_T tx_info;    				// Transfer information
    uint32_t src_addr;        				// Source (bus) address
    uint32_t dst_addr;     					// Destination (bus) address
    uint32_t tx_len;     					// Transfer length (in bytes)
    uint32_t stride;     					// 2D stride
    uint32_t next_cb;    					// Next DMAControlBlock (bus) address
	uint32_t debug;
    uint32_t _x1;
} DMA_CB_t __attribute__((aligned(32)));

typedef struct {
    DMA_CS_T cs;      						// DMA Channel Control and Status register
    uint32_t cb_addr; 						// DMA Channel Control Block Address
    DMA_TX_INFO_T tx_info;    				// Transfer information
    uint32_t src_addr;        				// Source (bus) address
    uint32_t dst_addr;     					// Destination (bus) address
    uint32_t tx_len;     					// Transfer length (in bytes)
    uint32_t stride;     					// 2D stride
    uint32_t next_cb;    					// Next DMAControlBlock (bus) address
	uint32_t debug;
} DMACtrlReg_t;

typedef union{
	struct{ volatile uint32_t EN0:1, EN1:1, EN2:1, EN3:1, EN4:1, EN5:1, EN6:1, EN7:1, EN8:1, EN9:1, EN10:1, EN11:1, EN12:1, EN13:1, EN14:1, _x1:9, PAGE:4, PAGELITE:4; };
	volatile uint32_t reg;
}DMAEnableReg_t;

typedef struct{
    volatile void* virtual_addr; 	// Virtual base address of the memory block
    dma_addr_t bus_addr;  			// Bus address of the memory block
	uint32_t size;
} DMAMemHandle_t;

uint8_t dma_init(void);
void dma_start(uint8_t write);
void dma_setup_CB(uint8_t id_cb, DMA_CB_t config);
DMAMemHandle_t* dma_malloc(unsigned int size);
void dma_free(volatile DMAMemHandle_t* mem);
uint8_t dma_freeAll(void);
void disp_dma(void);

#endif // !DMA_H