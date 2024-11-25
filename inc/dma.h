#ifndef DMA_H
#define DMA_H

#include "main.h"
#include "mailbox.h"

#define DMA_BASE		0x007000
#define DMA_chOffset	0x100		// sauf pour chan 15 qui est a : DMA_BASE + 0xE05000
#define CB_CNT			0x2
#define READ_CNT		0xFF
#define WRITE_CNT		0xFF
#define BUS_TO_PHYS(x) 	((x) & ~0xC0000000)

#define DMA_CS_FIELDS active:1, end:1, interrupt:1, dreq:1, paused:1, dreq_stop_dma:1, waiting_for_outstanding_writes:1, _x1:1,\
						error:1, _x2:7, priority:4, panic_priority:4, _x3:4, wait_for_outstanding_writes:1, disdebug:1, abort:1, reset:1
typedef union{
	struct{ volatile uint32_t DMA_CS_FIELDS; };
	volatile uint32_t value;
}DMA_CS_T;

typedef struct {
    DMA_CS_T cs;      						// DMA Channel Control and Status register
    uint32_t cb_addr; 						// DMA Channel Control Block Address
} DMACtrlReg_t;

#define DMA_TX_INFO_FIELDS inten:1, tdmode:1, _x1:1, wait_resp:1, dest_inc:1, dest_width:1, dest_dreq:1, dest_ignore:1,\
							src_inc:1, src_width:1,src_dreq:1, src_ignore:1, burst_len:4,permap:5, wait_cyc:5, no_wide_burst:1
typedef union{
	struct{ volatile uint32_t DMA_TX_INFO_FIELDS; };
	volatile uint32_t value;
}DMA_TX_INFO_T;

typedef struct{
    DMA_TX_INFO_T tx_info;    				// Transfer information
    uint32_t src_addr;        				// Source (bus) address
    uint32_t dst_addr;     					// Destination (bus) address
    uint32_t tx_len;     					// Transfer length (in bytes)
    uint32_t stride;     					// 2D stride
    uint32_t next_cb;    					// Next DMAControlBlock (bus) address
    uint32_t padding[2]; 					// 2-word padding
} DMA_CB_t __attribute__((aligned(32)));

typedef struct{
    void* virtual_addr; 					// Virutal base address of the memory block
    uint32_t bus_addr;  					// Bus address of the memory block
    uint32_t mb_handle; 					// Used internally by mailbox property interface
	uint32_t size;
} DMAMemHandle_t;

uint8_t init_dma(void);
void start_dma(uint8_t write);
void dma_setup_cb(uint32_t* src_addr, uint32_t* dst_addr, uint8_t tx_len, uint8_t loop, uint8_t write);
DMAMemHandle_t* dma_malloc(unsigned int size);
void dma_free(DMAMemHandle_t* mem);

#endif // !DMA_H