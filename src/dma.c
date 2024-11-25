#include "../inc/dma.h"

static volatile uint8_t* virt_baseDMA_ptr;
static volatile DMACtrlReg_t* virt_DMA_regs;
static volatile DMAEnableReg_t* virt_DMAEN_reg;
static volatile DMAMemHandle_t* dma_cbs;
static struct file *mailbox_file;

/*dmaengine et dma-map*/
static dma_cap_mask_t mask;
static struct dma_chan *chan;

uint8_t dma_init(void){
	virt_baseDMA_ptr = ioremap(PERIPH_BASE + DMA_BASE, DMA_REGS_SIZE);
	if(!virt_baseDMA_ptr){
		printk(KERN_ERR "erreur lors de map_segment pour la base des DMA\r\n");
		return 0;
	}

	dma_cap_zero(mask);
	dma_cap_set(DMA_SLAVE | DMA_PRIVATE, mask);
	chan = dma_request_channel(mask, NULL, NULL);
	if(!chan) {
		printk(KERN_ERR "Erreur : Impossible de récuperer un channel dma\n");
		return 0;
	}

	virt_DMA_regs = (DMACtrlReg_t *) (virt_baseDMA_ptr + (DMA_CHAN<<8));
	virt_DMAEN_reg = (DMAEnableReg_t *) (virt_baseDMA_ptr + DMA_ENOffsett);
	dma_cbs = dma_malloc(CB_CNT * sizeof(DMA_CB_t));
	if(!dma_cbs){
		printk(KERN_ERR "erreur lors du malloc pour dma_cbs\r\n");
		return 0;
	}
	return 1;
}

void dma_setup_CB(uint8_t id_cb, DMA_CB_t config){
	DMA_CB_t* cb = (DMA_CB_t*) ((uint32_t*) dma_cbs->virtual_addr + (id_cb-1));

	cb->tx_info.reg = config.tx_info.reg;
	cb->tx_len = config.tx_len;
	cb->src_addr = config.src_addr;
	cb->dst_addr = config.dst_addr;
	cb->stride = config.stride;
	cb->next_cb =(uint32_t) (!config.next_cb)? 0 : (DMA_CB_t*) ((uint32_t*) dma_cbs->virtual_addr + (config.next_cb-1));
}

void dma_start(uint8_t id_cb){
	if(virt_DMA_regs->cs.active){ 					// si le dma est deja actif on attend
		printk(KERN_DEBUG "virt_DMA_regs->cs.active = 1\r\n");
		disp_dma();
		while(!virt_DMA_regs->cs.active);	
	}
	virt_DMAEN_reg->EN5 = 1;
	virt_DMA_regs->cs.reset = 1;
	virt_DMA_regs->cb_addr = (uint32_t) dma_cbs->bus_addr + (id_cb-1);
	virt_DMA_regs->cs.end = 1;
	virt_DMA_regs->cs.active = 1;

	#if __DEBUG >= 1
		volatile DMA_CB_t* cb = (uint32_t*) dma_cbs->virtual_addr + (id_cb-1);
		printk(KERN_DEBUG "affichage de dma_cbs->virt_addr :\r\n");
		printk(KERN_DEBUG "cb->tx_info : ");
		for(uint8_t i=0; i<32; i++) printk(KERN_DEBUG "%d", (cb->tx_info.reg>>i)&1);
		printk(KERN_DEBUG "\r\ncb->src_addr : %x\r\n", cb->src_addr);
		printk(KERN_DEBUG "cb->dst_addr : %x\r\n", cb->dst_addr);
		printk(KERN_DEBUG "cb->tx_len : %x\r\n", cb->tx_len);
		printk(KERN_DEBUG "cb->stride : %x\r\n", cb->stride);
		printk(KERN_DEBUG "cb->next_cb : %x\r\n", cb->next_cb);
		printk(KERN_DEBUG "cb->debug : %x\r\n", cb->debug);
		sleep(1);
		printk(KERN_DEBUG "affichage de virt_DMA_regs->cb_addr :\r\n");

		printk(KERN_DEBUG "cb->tx_info : ");
		for(uint8_t i=0; i<32; i++) printk(KERN_DEBUG "%d", (virt_DMA_regs->tx_info.reg>>i)&1);
		printk(KERN_DEBUG "\r\ncb->src_addr : %x\r\n", virt_DMA_regs->src_addr);
		printk(KERN_DEBUG "cb->dst_addr : %x\r\n", virt_DMA_regs->dst_addr);
		printk(KERN_DEBUG "cb->tx_len : %x\r\n", virt_DMA_regs->tx_len);
		printk(KERN_DEBUG "cb->stride : %x\r\n", virt_DMA_regs->stride);
		printk(KERN_DEBUG "cb->next_cb : %x\r\n", virt_DMA_regs->next_cb);
		printk(KERN_DEBUG "cb->debug : ");
		for(int8_t i=28; i>=0; i--) printk(KERN_DEBUG "%d", (virt_DMA_regs->debug>>i)&1);
		printk(KERN_DEBUG "\r\n");
	#endif
}

DMAMemHandle_t* dma_malloc(unsigned int size){
	// if(!mailbox_file){
	// 	mailbox_file = mbox_open();
	// 	if(!mailbox_file){
	// 		printk(KERN_ERR "impossible d'ouvrire mbox\r\n");
	// 		return NULL;
	// 	}
	// }
    // Make `size` a multiple of PAGE_SIZE
    // size = ((size + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;
    DMAMemHandle_t* mem = (DMAMemHandle_t *) kmalloc(sizeof(mem), GFP_KERNEL);
	if(!mem){
		printk(KERN_ERR "Erreur : impossible d'allouer la mémoire pour le DMAMemHandele\r\n");
		return NULL;
	}
	mem->virtual_addr = dma_alloc_coherent(chan->device->dev, size, &(mem->bus_addr), GFP_KERNEL);
	printk(KERN_DEBUG "---------------------------DEBUG------------------------\r\n");
	if(!mem->virtual_addr){
		printk(KERN_ERR "Erreur : impossible d'allouer la mémoire unchached\r\n");
		kfree(mem);
		return NULL;
	}
    // mem->mb_handle = 	mem_alloc(mailbox_file, size, PAGE_SIZE, MEM_FLAG_L1_NONALLOCATING);
    // mem->bus_addr = 	mem_lock(mailbox_file, mem->mb_handle);
    // mem->virtual_addr = ioremap(BUS_TO_PHYS(mem->bus_addr), size);
    mem->size = size;

    return mem;
}

void dma_free(volatile DMAMemHandle_t* mem){
	dmam_free_coherent(chan->device->dev, mem->size, mem->virtual_addr, mem->bus_addr);
	kfree(mem);
    // iounmap(mem->virtual_addr);
    // mem_unlock(mailbox_file, mem->mb_handle);
    // mem_free(mailbox_file, mem->mb_handle);
    // mem->virtual_addr = NULL;
}

uint8_t dma_freeAll(void){
	dma_free(dma_cbs);
	iounmap(virt_baseDMA_ptr);
	dma_release_channel(chan);
	return 1;
}

#define __print_dma(reg) do{ printk(KERN_DEBUG #reg " : %3d   ", reg); }while(0);
void disp_dma(void){
	__print_dma(virt_DMAEN_reg->EN0);
	__print_dma(virt_DMAEN_reg->EN1);
	__print_dma(virt_DMAEN_reg->EN2);
	__print_dma(virt_DMAEN_reg->EN3);
	__print_dma(virt_DMAEN_reg->EN4);
	__print_dma(virt_DMAEN_reg->EN5);
	__print_dma(virt_DMAEN_reg->EN6);
	__print_dma(virt_DMAEN_reg->EN7);
	__print_dma(virt_DMAEN_reg->EN8);
	__print_dma(virt_DMAEN_reg->EN9);
	__print_dma(virt_DMAEN_reg->EN10);
	__print_dma(virt_DMAEN_reg->EN11);
	__print_dma(virt_DMAEN_reg->EN12);
	__print_dma(virt_DMAEN_reg->EN13);
	__print_dma(virt_DMAEN_reg->EN14);
	printk(KERN_DEBUG "\r\n");
	__print_dma(virt_DMAEN_reg->PAGE);
	__print_dma(virt_DMAEN_reg->PAGELITE);
	printk(KERN_DEBUG "\r\n");

	__print_dma(virt_DMA_regs->cs.disdebug);
	__print_dma(virt_DMA_regs->cs.wait_for_outstanding_writes);
	__print_dma(virt_DMA_regs->cs.panic_priority);
	__print_dma(virt_DMA_regs->cs.priority);
	__print_dma(virt_DMA_regs->cs.error);
	__print_dma(virt_DMA_regs->cs.wait_for_outstanding_writes);
	__print_dma(virt_DMA_regs->cs.dreq_stop_dma);
	__print_dma(virt_DMA_regs->cs.paused);
	__print_dma(virt_DMA_regs->cs.dreq);
	__print_dma(virt_DMA_regs->cs.interrupt);
	__print_dma(virt_DMA_regs->cs.end);
	__print_dma(virt_DMA_regs->cs.active);
	printk(KERN_DEBUG "\r\n");

	__print_dma(virt_DMA_regs->cb_addr);
	printk(KERN_DEBUG "\r\n");

	printk(KERN_DEBUG "cb->tx_info : ");
	for(uint8_t i=0; i<32; i++) printk(KERN_DEBUG "%d", (virt_DMA_regs->tx_info.reg>>i)&1);
	printk(KERN_DEBUG "\r\ncb->src_addr : %x\r\n", virt_DMA_regs->src_addr);
	printk(KERN_DEBUG "cb->dst_addr : %x\r\n", virt_DMA_regs->dst_addr);
	printk(KERN_DEBUG "cb->tx_len : %x\r\n", virt_DMA_regs->tx_len);
	printk(KERN_DEBUG "cb->stride : %x\r\n", virt_DMA_regs->stride);
	printk(KERN_DEBUG "cb->next_cb : %x\r\n", virt_DMA_regs->next_cb);
	printk(KERN_DEBUG "cb->debug : ");
	for(int8_t i=28; i>=0; i--) printk(KERN_DEBUG "%d", (virt_DMA_regs->debug>>i)&1);
	printk(KERN_DEBUG "\r\n");
}