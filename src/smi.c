//==============
//
// SMI Interface 
//
//==============
//
// Basic routines to get SMI running 
// Not optimised for speed!
//

#include "smi.h"

// 
// Main access pointer to SMI peripheral
//
extern volatile uint32_t 		*virt_gpio_regs;
volatile uint32_t 				*virt_clk_regs;
volatile uint32_t 				*virt_smi_regs;
static volatile SMI_CS_REG_T  	*smi_cs;
static volatile SMI_L_REG_T   	*smi_l;
static volatile SMI_A_REG_T   	*smi_a;
static volatile SMI_D_REG_T   	*smi_d;
static volatile SMI_DC_REG_T 	*smi_dc;
static volatile SMI_DSR_REG_T 	*smi_dsr[4];
static volatile SMI_DSW_REG_T 	*smi_dsw[4];
static volatile SMI_DCS_REG_T 	*smi_dcs;
static volatile SMI_DA_REG_T 	*smi_da;
static volatile SMI_DD_REG_T 	*smi_dd;

uint8_t init_smi_clk(void){
	volatile uint32_t periph_addr = bcm_host_get_peripheral_address();
	
	printf("addr smi = %x\r\n",periph_addr + SMI_BASE);
	virt_smi_regs = (uint32_t*) map_segment((void *) periph_addr + SMI_BASE, PAGE_SIZE);
	if(!virt_smi_regs){
		printf("erreur lors de map_segment pour SMI\r\n");
		return 0;
	}

	smi_cs = (SMI_CS_REG_T*) &virt_smi_regs[0];
	smi_l = (SMI_L_REG_T*) &virt_smi_regs[1];
	smi_a = (SMI_A_REG_T*) &virt_smi_regs[2];
	smi_d = (SMI_D_REG_T*) &virt_smi_regs[3];
	smi_dsr[0] = (SMI_DSR_REG_T*) &virt_smi_regs[4];
	smi_dsw[0] = (SMI_DSW_REG_T*) &virt_smi_regs[5];
	smi_dsr[1] = (SMI_DSR_REG_T*) &virt_smi_regs[6];
	smi_dsw[1] = (SMI_DSW_REG_T*) &virt_smi_regs[7];
	smi_dsr[2] = (SMI_DSR_REG_T*) &virt_smi_regs[8];
	smi_dsw[2] = (SMI_DSW_REG_T*) &virt_smi_regs[9];
	smi_dsr[3] = (SMI_DSR_REG_T*) &virt_smi_regs[10];
	smi_dsw[3] = (SMI_DSW_REG_T*) &virt_smi_regs[11];
	smi_dc = (SMI_DC_REG_T*) &virt_smi_regs[12];
	smi_dcs = (SMI_DCS_REG_T*) &virt_smi_regs[13];
	smi_da = (SMI_DA_REG_T*) &virt_smi_regs[14];
	smi_dd = (SMI_DD_REG_T*) &virt_smi_regs[15];

	printf("addr clk = %x\r\n",periph_addr + CLK_BASE);
	virt_clk_regs = (uint32_t*) map_segment((void *) periph_addr + CLK_BASE, PAGE_SIZE);
	if(!virt_clk_regs){
		printf("erreur lors de map_segment pour CLK\r\n");
		return 0;
	}
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
uint8_t setup_smi(void){
	volatile unsigned int *smi_clk;
	
	if(!(virt_smi_regs && virt_clk_regs)){
		if(!init_smi_clk()){
			printf("init SMI et Clocks failed\r\n");
			return 0;
		}
	};
	if(!virt_gpio_regs){
		if(!init_gpio()){
			printf("init Gpio failed\r\n");
			return 0;
		}
	}
	
	// Set GPIO 0..25 to SMI mode
	for (uint8_t i=0; i<26; i++){
		INP_GPIO(i);
		SET_GPIO_ALT(i,1);
	}

	// Set SMI clock 
	smi_clk = virt_clk_regs+(0xB0>>2);		// smi clk offset
	smi_clk[0] = 0x5A << 24; 				// Off

	smi_clk[1] = 0x5A << 24 | 2<<12;		// psswd | int div
	smi_clk[0] = 0x5A << 24 | 1<<4 | 6;		// psswd | enable | clk src
	
	for(uint8_t i=0;i<4;i++){
		smi_dsr[i]->rwidth = 1;
		smi_dsr[i]->rsetup = 9;
		smi_dsr[i]->mode68 = 0;
		smi_dsr[i]->rhold = 45;
		smi_dsr[i]->rpaceall = 1;
		smi_dsr[i]->rpace = 1;
		smi_dsr[i]->rstrobe = 21;

		smi_dsw[i]->wwidth = 1;
		smi_dsw[i]->wsetup = 9;
		smi_dsw[i]->wformat = 0;
		smi_dsw[i]->whold = 45;
		smi_dsw[i]->wpaceall = 1;
		smi_dsw[i]->wpace = 1;
		smi_dsw[i]->wstrobe = 21;
	}

	// Set channel 1 (index 2,3) for DMA mode
	smi_dsr[1]->rdreq = 1;
	smi_dsw[1]->wdreq = 1;

	// Using SMI D16/D17 as DREQ/DACK pins
	smi_dc->dmap = 1;
			
	return 1;
}

//
// Setup one of eight interface timings
// 
uint8_t set_smi_timing(uint8_t channel, uint8_t read, uint8_t setup, uint8_t strobe, uint8_t hold, uint8_t pace){
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
	smi_da->dev = timing& 0b11;
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
	smi_cs->enable = 14;
	smi_cs->start = 1;
	
	while(words){ 
		while(smi_cs->rxd){
			*data++ = (uint16_t) smi_d->data;
			words--;
		}
		printf("%d\n",words);
	}

	// Busy wait till done
	while(--time_out && !(smi_cs->done));
	if (time_out==0) return 0;   
	
	// clear done bit, keep enabled
	smi_cs->done = 1;
	smi_cs->enable = 1;
	
	return 1;
}

/*
// *****************************************
// Simple DMA Setup Routine
// *****************************************


void common_dma_setup  ( const uint32_t channel,                         // dma channel 0 to 15
                        const uint32_t source_addr,                     // 30 bit source address
                        const uint32_t dest_addr,                       // 30 bit destination address
                        const uint32_t src_stride,                      // 2d source stride
                        const uint32_t dst_stride,                      // 2d destination stride
                        const uint32_t transfer_length,                 // length in bytes
                        const uint32_t control_block_addr,              // address of 256 bit aligned block of memory 256 bits long to store control block in
                        const uint32_t next_control_block_addr,         // address of the next control block to load (0 if you want the dma to stop)
                        const uint32_t transfer_info                    // transfer info control reg
                      )
{

   // write the control block
   *((int*)control_block_addr + 0) = transfer_info;
   *((int*)control_block_addr + 1) = source_addr;
   *((int*)control_block_addr + 2) = dest_addr;
   *((int*)control_block_addr + 3) = transfer_length;
   *((int*)control_block_addr + 4) = ((dst_stride & 0xffff) << 16) | src_stride & 0xffff;
   *((int*)control_block_addr + 5) = next_control_block_addr;

   // channel 15 is in the VPU
   if (channel == 15) {
      DMA15_CS        = 0x0;
      DMA15_CS        = (1<<DMA15_CS_INT_LSB) | (1<<DMA15_CS_END_LSB);
      DMA15_CONBLK_AD = (int)control_block_addr;
   } else {
      DMA_CS(channel)        = 0x0 ;                                        // make sure dma is stopped
      DMA_CS(channel)        = (1<<DMA_0_CS_INT_LSB) | (1<<DMA_0_CS_END_LSB); // clear interrupts and end flag by writing a 1 to them
      DMA_CONBLK_AD(channel) = (int)control_block_addr;                     // give the dma the control block
   }


   // it wont start until we set the active bit

}


// *****************************************
// start the dma channel
// *****************************************
void common_dma_start ( const uint32_t channel ) {

   if (channel == 15) {
      DMA15_CS |= (1<<DMA15_CS_ACTIVE_LSB);
   } else {
      DMA_CS(channel) |= (1<<DMA_8_CS_ACTIVE_LSB);
   }

}

*/

/*
// Find out what revision board the Raspberry Pi is
// Using the file '/proc/cpuinfo' for that.
// returns 
//  0 : could not tell 
//  1 : rev 1
//  2 : rev 2
//  3 : B+
//
int pi_revision()
{
   FILE *fp;
   int  revision, match, number;
   char text[128]; // holds one line of file

   revision = 0;
   // Open the file with the CPU info
   fp = fopen("/proc/cpuinfo","r");
   if (fp)
   { // Find the line which starts 'Revision'
     while (fgets(text, 128, fp)) // get one line of text from file
     {
       if (!strncmp(text,"Revision",8)) // strncmp returns 0 if string matches
       { // Get the revision number from the text
      	 match = sscanf(text,"%*s : %0X",&number); // rev num is after the :
      	 if (match == 1)
      	   { // Yes, we have a revision number
      	     if (number >= 10)
      	       revision = 3;
      	     else
      	     if (number >= 4)
      	       revision = 2;
      	     else
      	       revision = 1;
      	   } // have number
                break; // no use in reading more lines, so break out of the while
       } // have revision text
     } // get line from file
     fclose(fp);
   } // Have open file

   return revision;
} // pi_revision
 */