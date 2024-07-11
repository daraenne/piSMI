#ifndef SMI_H
#define SMI_H
//
// BCM283x SMI interface 
// Derived from Documentation
// GVL 15-Oct-2014 
//

#include "main.h"
#include "gpio.h"

// Registers offset in CPU map 
#define SMI_BASE            	0x600000
#define CLK_BASE            	0x101000

#define SMI_CS_FIELDS 	enable:1, done:1, active:1, start:1, clear:1, write:1, _x1:2,\
    					teen:1, intd:1, intt:1, intr:1, pvmode:1, seterr:1, pxldat:1, edreq:1,\
    					_x2:8, _x3:1, aferr:1, txw:1, rxr:1, txd:1, rxd:1, txe:1, rxf:1
typedef union{
	struct{ volatile uint32_t SMI_CS_FIELDS; };
	volatile uint32_t value;
}SMI_CS_REG_T;

typedef union{
	struct{ volatile uint32_t len:32; };
	volatile uint32_t value;
}SMI_L_REG_T;

typedef union{
	struct{ volatile uint32_t addr:6, _x1:2, dev:2; };
	volatile uint32_t value;
}SMI_A_REG_T;

typedef union{
	struct{ volatile uint32_t data:32; };
	volatile uint32_t value;
}SMI_D_REG_T;

typedef union{
	struct{ volatile uint32_t reqw:6, reqr:6, panicw:6, panicr:6, dmap:1, _x1:3, dmaen:1; };
	volatile uint32_t value;
}SMI_DC_REG_T;

typedef union{
	struct{ volatile uint32_t rstrobe:7, rdreq:1, rpace:7, rpaceall:1, rhold:6, fsetup:1, mode68:1, rsetup:6, rwidth:2; };
	volatile uint32_t value;
}SMI_DSR_REG_T;

typedef union{
	struct{ volatile uint32_t wstrobe:7, wdreq:1, wpace:7, wpaceall:1, whold:6, wswap:1, wformat:1, wsetup:6, wwidth:2; };
	volatile uint32_t value;
}SMI_DSW_REG_T;

typedef union{
	struct{ volatile uint32_t enable:1, start:1, done:1, write:1; };
	volatile uint32_t value;
}SMI_DCS_REG_T;

typedef union{
	struct{ volatile uint32_t addr:6, _x1:2, dev:2; };
	volatile uint32_t value;
}SMI_DA_REG_T;

typedef union{
	struct{ volatile uint32_t data:32; };
	volatile uint32_t value;
}SMI_DD_REG_T;

typedef union{
	struct{ volatile uint32_t fcnt:6, _x1:2, flvl:6; };
	volatile uint32_t value;
}SMI_FLVL_REG_T;

// Setup SMI
// return 0 on fail
// return 1 on OK 
uint8_t setup_smi(void);
uint8_t init_smi_clk(void);

//
// Setup one of four interface timings
// read or write
//
// return 1 on OK
// return 0 on failure 
// 
uint8_t set_smi_timing(uint8_t channel, uint8_t read, uint8_t setup, uint8_t strobe, uint8_t hold, uint8_t pace);

/**
 * Perform direct write
 * 
 * return 0 on fail
 * return 1 on OK
 */
// Perform direct write
//
// return 0 on fail
// return 1 on OK 
// 
uint8_t smi_direct_write(uint8_t timing, uint32_t data, uint32_t address, uint32_t time_out);

//
// Perform direct read
// 
// return 0 on fail
// return 1 on OK 
// 
uint8_t smi_direct_read(uint8_t timing, uint32_t* data, uint32_t address, uint32_t time_out);


//
// Perform block write
// 
uint8_t smi_block_write(uint8_t timing, uint32_t words, uint16_t *data, uint8_t address, uint32_t time_out);
                    
//
// Perform block read
// 
uint8_t smi_block_read(uint8_t timing, uint32_t words, uint16_t *data, uint8_t address, uint32_t time_out);

#endif // !SMI_H