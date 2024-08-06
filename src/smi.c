#include "../inc/smi.h"

// volatile DMAMemHandle_t* 		readBuff;
// volatile DMAMemHandle_t* 		writeBuff;
static volatile uint32_t*		virt_clk_regs;
static volatile uint32_t*		virt_smi_regs;
static volatile SMI_CS_REG_T*	smi_cs;
static volatile SMI_L_REG_T*	smi_l;
static volatile SMI_A_REG_T*	smi_a;
static volatile SMI_D_REG_T*	smi_d;
static volatile SMI_DC_REG_T*	smi_dc;
static volatile SMI_DSR_REG_T*	smi_dsr[4];
static volatile SMI_DSW_REG_T*	smi_dsw[4];
static volatile SMI_DCS_REG_T*	smi_dcs;
static volatile SMI_DA_REG_T*	smi_da;
static volatile SMI_DD_REG_T*	smi_dd;
// static DMA_CB_t 				dma_cb_config;

uint8_t smi_init(void){
	virt_smi_regs = (uint32_t*) ioremap(PERIPH_BASE + SMI_BASE, SMI_REGS_SIZE);
	if(!virt_smi_regs){
		printk(KERN_ERR "erreur lors de map_segment pour SMI\r\n");
		return 0;
	}
	
	// readBuff = dma_malloc(READ_CNT * sizeof(uint32_t));
	// if(NULL == readBuff){
	// 	printf("erreur lors du malloc pour readBuff\r\n");
	// 	return 0;
	// }
	// writeBuff = dma_malloc(WRITE_CNT * sizeof(uint32_t));
	// if(NULL == writeBuff){
	// 	printf("erreur lors du malloc pour writeBuff\r\n");
	// 	return 0;
	// }

	smi_cs = (SMI_CS_REG_T*) &virt_smi_regs[0];
	smi_l = (SMI_L_REG_T*) &virt_smi_regs[1];
	smi_a = (SMI_A_REG_T*) &virt_smi_regs[2];
	smi_d = (SMI_D_REG_T*) &virt_smi_regs[3];
	for(uint8_t i=0; i<4; i++){
		smi_dsr[i] = (SMI_DSR_REG_T*) &virt_smi_regs[(2*i)+4];
		smi_dsw[i] = (SMI_DSW_REG_T*) &virt_smi_regs[(2*i)+5];
	}
	smi_dc = (SMI_DC_REG_T*) &virt_smi_regs[12];
	smi_dcs = (SMI_DCS_REG_T*) &virt_smi_regs[13];
	smi_da = (SMI_DA_REG_T*) &virt_smi_regs[14];
	smi_dd = (SMI_DD_REG_T*) &virt_smi_regs[15];

	virt_clk_regs = (uint32_t*) ioremap(PERIPH_BASE + CLK_BASE, SMICLK_REGS_BASE);
	if(!virt_clk_regs){
		printk(KERN_ERR "erreur lors de ioremap pour la CLK du smi\r\n");
		return 0;
	}

	// dma_cb_config.tx_info.no_wide_burst = 1;
	// dma_cb_config.tx_info.wait_resp = 1;
	// dma_cb_config.tx_info.dest_dreq = 1;
	// dma_cb_config.tx_info.permap = 4;
	return 1;
}

//
// Set up basic SMI mode 
// channel 0 is for 'PIO' mode
// channel 1 is for 'DMA' mode 
//
// return 1 on OK
// return 0 on failure 
//
uint8_t smi_setup(void){
	volatile unsigned int* smi_clk;
	
	if(!(virt_smi_regs && virt_clk_regs)){
		if(!smi_init()){
			printk(KERN_ERR "probleme lors de l'init du SMI et et de sa Clocks\r\n");
			return 0;
		}
	};

	// Set GPIO 2..25 to SMI mode (gpio 0, 1 et apres 27 ne sont pas disponible sur la rpi4 et gpio 26 et 27 son reservé en alt1)
	for (uint8_t i=2; i<26; i++){
		if(!gpio_config(i,alt1)){
			printk(KERN_ERR "probleme setup des gpio en mode SMI\r\n");
			return 0;
		}
	}

	// Set SMI clock 
	smi_clk = virt_clk_regs+(0xB0>>2);		// smi clk offset
	smi_clk[0] = 0x5A << 24; 				// Off

	smi_clk[1] = 0x5A << 24 | 2<<12;		// psswd | int div
	smi_clk[0] = 0x5A << 24 | 1<<4 | 6;		// psswd | enable | clk src
	
	for(uint8_t i=0;i<4;i++){
		smi_set_timing(i, 0, 9, 21, 45, 1);
		smi_dsr[i]->rwidth = 1;
		smi_dsr[i]->mode68 = 0;
		smi_dsr[i]->rpaceall = 1;
		smi_dsr[i]->rdreq = 0;

		smi_set_timing(i, 1, 9, 21, 45, 1);
		smi_dsw[i]->wwidth = 1;
		smi_dsw[i]->wformat = 0;
		smi_dsw[i]->wpaceall = 1;
		smi_dsw[i]->wdreq = 0;
	}

	// Using SMI D16/D17 as DREQ/DACK pins
	smi_dc->dmap = 1;
			
	return 1;
}

//
// Setup one of eight interface timings
// 
uint8_t smi_set_timing(uint8_t channel, uint8_t read, uint8_t setup, uint8_t strobe, uint8_t hold, uint8_t pace){
	if(read){
		smi_dsr[channel]->rsetup = setup;
		smi_dsr[channel]->rstrobe = strobe;
		smi_dsr[channel]->rhold = hold;
		smi_dsr[channel]->rpace = pace;
	}else{
		smi_dsw[channel]->wsetup = setup;
		smi_dsw[channel]->wstrobe = strobe;
		smi_dsw[channel]->whold = hold;
		smi_dsw[channel]->wpace = pace;
	}
	return 1;
}

//
// Perform direct write
// 
uint8_t smi_direct_write(uint8_t timing, uint32_t data, uint32_t address, uint32_t time_out){
	// clear done bit if set
	if(smi_dcs->done) smi_dcs->done = 1;

	// Start write transfer 
	smi_da->dev = timing& 0b11;
	smi_da->addr = address;
	smi_d->data = data;
	smi_dcs->write = 1;
	smi_dcs->enable = 1;
	smi_dcs->start = 1;

	// busy wait till done
	while(--time_out && !(smi_cs->done));
	if (time_out==0) return 0;

	// clear done bit 
	smi_cs->done = 1;

	return 1;
}

// 
// Perform direct read
// 
uint8_t smi_direct_read(uint8_t timing, uint32_t* data, uint32_t address, uint32_t time_out){
	// clear done bit if set 
	if(smi_dcs->done) smi_dcs->done = 1;
	
	// Start read transfer 
	smi_da->dev = timing & 0b11;
	smi_da->addr = address;
	smi_dcs->write = 0;
	smi_dcs->enable = 1;
	smi_dcs->start = 1;
	
	// busy wait till done
	while(--time_out && !(smi_cs->done));
	if (time_out==0) return 0;    
	
	// get the data 
	*data = smi_dd->data;

	// clear done bit  
	smi_cs->done = 1;
	
	// report success
	return 1;
}


//
// Perform block write
// 
uint8_t smi_block_write(uint8_t timing, uint32_t words, uint16_t *data, uint8_t address, uint32_t time_out){
	// debug: clear FIFO 
	smi_cs->clear = 1;
	
	// clear done bit, enable for writes 
	smi_cs->write = 1;
	smi_cs->done = 1;
	smi_cs->enable = 1;
	// Set address & length 
	smi_a->dev = timing & 0b11;
	smi_a->addr = address;
	smi_l->len = words;
	
	// pre-pump data into FIFO 
	// until full or run our of data 
	while(words && smi_cs->txd){ 
		smi_d->data = (int)*data++;
		words--;
	}
	
	// Start transfer
	smi_cs->enable = 1;
	smi_cs->write = 1;
	smi_cs->start = 1;
	
	// busy wait, pumping data into FIFO
	uint32_t tmpBuff = time_out;
	while (--time_out && words){
		if(smi_cs->txd){
			smi_d->data = (int)*data++;
			words--;
		}
	}
	if (time_out==0) return 0;     
	
	// Busy wait till done
	time_out = tmpBuff;
	while(--time_out && !(smi_cs->done));
	if (time_out==0) return 0;
	
	// clear done bit, keep enabled 
	smi_cs->done = 1;
	smi_cs->enable = 1;
	
	return 1;
}

// uint8_t smi_dma_write(uint8_t timing, uint32_t bytes, uint16_t *data, uint8_t address, uint8_t loop, uint8_t treshold){
// 	smi_cs->clear = 1;
// 	smi_cs->enable = 1;
// 	smi_cs->write = 1;
// 	smi_l->len = bytes / sizeof(uint16_t);
// 	smi_a->dev = timing & 0b11;
// 	smi_a->addr = address;
// 	smi_dc->dmaen = 1;
// 	smi_dc->dmap = 1;
// 	smi_dc->reqw = treshold;
// 	smi_dsw[timing]->wdreq = 0;
	

// 	// for(uint32_t i=0; i<bytes/2; i++) ((uint16_t*)writeBuff->virtual_addr)[i] = data[i];
// 	memcpy(writeBuff->virtual_addr, data, bytes);
// 	dma_cb_config.tx_info.dest_inc = 0;
// 	dma_cb_config.tx_info.src_inc = 1;
// 	dma_cb_config.src_addr = writeBuff->bus_addr;
// 	dma_cb_config.dst_addr = (uint32_t *) (BUS_BASE + SMI_BASE + SMI_DATA_REG);
// 	dma_cb_config.tx_len = bytes;
// 	dma_cb_config.next_cb = 0;
// 	dma_setup_CB(1, dma_cb_config);
// 	start_dma(1);

// 	smi_cs->start = 1;
// 	#if __DEBUG>=1
// 		sleep(1);
// 		disp_smi();
// 	#endif
// 	return 1;
// }

//
// Perform block read
// When calling this the SMI immediately starts reading
// Thus your device must have data available to read 
// Or the smi channel must be set up to use DMA mode (DREQ/DACK)
// This code need optimisation for the CPU to keep up at high speed.
// Or needs a DMA channel assigned
// 
uint8_t smi_block_read(uint8_t timing, uint32_t words, uint16_t *data, uint8_t address, uint32_t time_out){
	// clear done bit, enable for writes 
	smi_cs->done = 1;
	smi_cs->enable = 1;
	smi_cs->clear = 1;
	
	// Set address & length
	smi_a->dev = timing & 0b11;
	smi_a->addr = address;
	smi_l->len = words;
		
	// Start transfer
	smi_cs->enable = 1;
	smi_cs->start = 1;
	
	while(words){ 
		while(smi_cs->rxd){
			*data++ = (uint16_t) smi_d->data;
			words--;
		}
	}

	// Busy wait till done
	while(--time_out && !(smi_cs->done));
	if (time_out==0) return 0;   
	
	// clear done bit, keep enabled
	smi_cs->done = 1;
	smi_cs->enable = 1;
	
	return 1;
}

void smi_freeAll(void){
	// dma_free(readBuff);
	// dma_free(writeBuff);
	if(!virt_clk_regs) iounmap(virt_clk_regs);
	if(!virt_smi_regs) iounmap(virt_smi_regs);
}

// Display DMA registers
#define __print_smi(reg) do{if(reg) printk(KERN_DEBUG #reg " : %3d   ", reg);}while(0)
void smi_disp(void){
    __print_smi(smi_cs->rxf);
    __print_smi(smi_cs->txe);
    __print_smi(smi_cs->rxd);
    __print_smi(smi_cs->txd);
    __print_smi(smi_cs->rxr);
    __print_smi(smi_cs->txw);
    __print_smi(smi_cs->aferr);
    __print_smi(smi_cs->prdy);
    __print_smi(smi_cs->edreq);
    __print_smi(smi_cs->pxldat);
    __print_smi(smi_cs->seterr);
    __print_smi(smi_cs->pvmode);
    __print_smi(smi_cs->intr);
    __print_smi(smi_cs->intt);
    __print_smi(smi_cs->intd);
    __print_smi(smi_cs->teen);
    __print_smi(smi_cs->pad);
    __print_smi(smi_cs->write);
    __print_smi(smi_cs->clear);
    __print_smi(smi_cs->start);
    __print_smi(smi_cs->active);
    __print_smi(smi_cs->done);
    __print_smi(smi_cs->enable);
	printk("\r\n");
    __print_smi(smi_l->len);
	printk("\r\n");
    __print_smi(smi_a->dev);
    __print_smi(smi_a->addr);
	printk("\r\n");
    if(smi_cs->rxd) __print_smi(smi_d->data);
	else printk("no data to read");
	printk("\r\n");
    __print_smi(smi_dc->dmaen);
    __print_smi(smi_dc->dmap);
    __print_smi(smi_dc->panicr);
    __print_smi(smi_dc->panicw);
    __print_smi(smi_dc->reqr);
    __print_smi(smi_dc->reqw);
	printk("\r\n");
	for(uint8_t i=0; i<4; i++){
    	__print_smi(smi_dsr[i]->rwidth);
    	__print_smi(smi_dsr[i]->rsetup);
    	__print_smi(smi_dsr[i]->mode68);
    	__print_smi(smi_dsr[i]->fsetup);
    	__print_smi(smi_dsr[i]->rhold);
    	__print_smi(smi_dsr[i]->rpaceall);
    	__print_smi(smi_dsr[i]->rpace);
    	__print_smi(smi_dsr[i]->rdreq);
    	__print_smi(smi_dsr[i]->rstrobe);
	printk("\r\n");
	}

	for(uint8_t i=0; i<4; i++){
    	__print_smi(smi_dsw[i]->wwidth);
    	__print_smi(smi_dsw[i]->wsetup);
    	__print_smi(smi_dsw[i]->wformat);
    	__print_smi(smi_dsw[i]->wswap);
    	__print_smi(smi_dsw[i]->whold);
    	__print_smi(smi_dsw[i]->wpaceall);
    	__print_smi(smi_dsw[i]->wpace);
    	__print_smi(smi_dsw[i]->wdreq);
    	__print_smi(smi_dsw[i]->wstrobe);
	printk("\r\n");
	}

    __print_smi(smi_dcs->write);
    __print_smi(smi_dcs->done);
    __print_smi(smi_dcs->start);
    __print_smi(smi_dcs->enable);
	printk("\r\n");
    __print_smi(smi_da->dev);
    __print_smi(smi_da->addr);
	printk("\r\n");
    __print_smi(smi_dd->data);
	printk("\r\n");
	volatile unsigned int* smi_clk = virt_clk_regs+(0xB0>>2);
	__print_smi(smi_clk[0]);
	printk("\r\n");
	__print_smi(smi_clk[1]);
	printk("\r\n");
}