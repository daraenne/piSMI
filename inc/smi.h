#ifndef SMI_H
#define SMI_H

#include <linux/bits.h>
#include "../inc/gpio.h"
#include "../inc/dma.h"

#define __DEBUG 0

#define SMI_BASE        	0x600000
#define SMI_DATA_REG		0x0C
#define CLK_BASE        	0x101000
#define SMI_REGS_SIZE		0x40
#define SMICLK_REGS_BASE	0x08

#define FIFO_SIZE			1024
#define READ_BUFFSIZE		FIFO_SIZE
#define WRITE_BUFFSIZE		FIFO_SIZE

#define SMI_CS_FIELDS 	enable:1, done:1, active:1, start:1, clear:1, write:1, pad:2,\
    					teen:1, intd:1, intt:1, intr:1, pvmode:1, seterr:1, pxldat:1, edreq:1,\
    					_x1:8, prdy:1, aferr:1, txw:1, rxr:1, txd:1, rxd:1, txe:1, rxf:1
typedef union{
	struct{ volatile uint32_t SMI_CS_FIELDS; };
	volatile uint32_t reg;
}SMI_CS_REG_T;

typedef union{
	struct{ volatile uint32_t len:32; };
	volatile uint32_t reg;
}SMI_L_REG_T;

typedef union{
	struct{ volatile uint32_t addr:6, _x1:2, dev:2; };
	volatile uint32_t reg;
}SMI_A_REG_T;

typedef union{
	struct{ volatile uint32_t data:32; };
	volatile uint32_t reg;
}SMI_D_REG_T;

typedef union{
	struct{ volatile uint32_t reqw:6, reqr:6, panicw:6, panicr:6, dmap:1, _x1:3, dmaen:1; };
	volatile uint32_t reg;
}SMI_DC_REG_T;

typedef union{
	struct{ volatile uint32_t rstrobe:7, rdreq:1, rpace:7, rpaceall:1, rhold:6, fsetup:1, mode68:1, rsetup:6, rwidth:2; };
	volatile uint32_t reg;
}SMI_DSR_REG_T;

typedef union{
	struct{ volatile uint32_t wstrobe:7, wdreq:1, wpace:7, wpaceall:1, whold:6, wswap:1, wformat:1, wsetup:6, wwidth:2; };
	volatile uint32_t reg;
}SMI_DSW_REG_T;

typedef union{
	struct{ volatile uint32_t enable:1, start:1, done:1, write:1; };
	volatile uint32_t reg;
}SMI_DCS_REG_T;

typedef union{
	struct{ volatile uint32_t addr:6, _x1:2, dev:2; };
	volatile uint32_t reg;
}SMI_DA_REG_T;

typedef union{
	struct{ volatile uint32_t data:32; };
	volatile uint32_t reg;
}SMI_DD_REG_T;

typedef union{
	struct{ volatile uint32_t fcnt:6, _x1:2, flvl:6; };
	volatile uint32_t reg;
}SMI_FLVL_REG_T;

uint8_t smi_init(void);
uint8_t smi_setup(void);
void smi_freeAll(void);
uint8_t smi_set_timing(uint8_t channel, uint8_t read, uint8_t setup, uint8_t strobe, uint8_t hold, uint8_t pace);
uint8_t smi_direct_write(uint8_t timing, uint32_t data, uint32_t address, uint32_t time_out);
uint8_t smi_direct_read(uint8_t timing, uint32_t* data, uint32_t address, uint32_t time_out);
uint8_t smi_block_write(uint8_t timing, uint32_t words, uint16_t *data, uint8_t address, uint32_t time_out);
uint8_t smi_dma_write(uint8_t timing, uint32_t bytes, uint16_t *data, uint8_t address, uint8_t loop, uint8_t treshold);
uint8_t smi_block_read(uint8_t timing, uint32_t words, uint16_t *data, uint8_t address, uint32_t time_out);
void smi_disp(void);

#endif // !SMI_H