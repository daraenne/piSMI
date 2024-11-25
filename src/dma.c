#include "dma.h"

static volatile uint8_t* virt_baseDMA_ptr;
static volatile DMACtrlReg_t* virt_DMA_regs;
static volatile DMAMemHandle_t* dma_cbs;
volatile DMAMemHandle_t* readBuff;
volatile DMAMemHandle_t* writeBuff;
static int mailbox_fd=0;

uint8_t init_dma(void){
	// on utilise que le channel 5 du DMA
	uint8_t dma_chan = 5;
	virt_baseDMA_ptr = map_segment(DMA_BASE, PAGE_SIZE);
	if(!virt_baseDMA_ptr){
		printf("erreur lors de map_segment pour la base des DMA\r\n");
		return NULL;
	}

	virt_DMA_regs = (DMACtrlReg_t *) (virt_baseDMA_ptr + dma_chan * DMA_chOffset);

	dma_cbs = dma_malloc(CB_CNT * sizeof(DMA_CB_t));
	if(NULL == dma_cbs){
		printf("erreur lors du malloc pour dma_cbs\r\n");
		return NULL;
	}
	readBuff = dma_malloc(READ_CNT * sizeof(uint16_t));
	if(NULL == readBuff){
		printf("erreur lors du malloc pour readBuff\r\n");
		return NULL;
	}
	writeBuff = dma_malloc(WRITE_CNT * sizeof(uint16_t));
	if(NULL == writeBuff){
		printf("erreur lors du malloc pour writeBuff\r\n");
		return NULL;
	}

	return 1;
}

void dma_setup_cb(uint32_t* src_addr, uint32_t* dst_addr, uint8_t tx_len, uint8_t loop, uint8_t write){
	DMA_CB_t* cb = dma_cbs[!write].virtual_addr;
	cb->tx_info.no_wide_burst = 1;
	cb->tx_info.wait_resp = 1;
	cb->src_addr = src_addr;
	cb->dst_addr = src_addr;
	cb->tx_len = tx_len;
	cb->next_cb = (loop)? dma_cbs[!write].bus_addr : 0;
}

void start_dma(uint8_t write){
	virt_DMA_regs->cb_addr = &dma_cbs[!write].bus_addr;
	virt_DMA_regs->cs.end = 1;
	virt_DMA_regs->cs.active = 1;
	printf("virt_DMA_regs : \r\n");
	printf("TI = %x\r\n", virt_DMA_regs->cb_addr);
	printf("TI = %x\r\n", dma_cbs[!write].bus_addr);
}

DMAMemHandle_t* dma_malloc(unsigned int size){
	if(!mailbox_fd) mailbox_fd = mbox_open();
	
    // Make `size` a multiple of PAGE_SIZE
    size = ((size + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;

    DMAMemHandle_t* mem = (DMAMemHandle_t *) malloc(sizeof(DMAMemHandle_t));

    mem->mb_handle = 	mem_alloc(mailbox_fd, size, PAGE_SIZE, (1 << 2) | (2 << 2));
    mem->bus_addr = 	mem_lock(mailbox_fd, mem->mb_handle);
    mem->virtual_addr = mapmem(BUS_TO_PHYS(mem->bus_addr), size);
    mem->size = size;

    return mem;
}

void dma_free(DMAMemHandle_t* mem){
    unmapmem(mem->virtual_addr, mem->size);
    mem_unlock(mailbox_fd, mem->mb_handle);
    mem_free(mailbox_fd, mem->mb_handle);
    mem->virtual_addr = NULL;
}