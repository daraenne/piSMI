#ifndef DMA_H
#define DMA_H

#include "main.h"

#define DMA_BASE				0x007000
#define DMA_chOffset			0x100		// sauf pour chan 15 qui est a : DMA_BASE + 0xE05000

// https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface
#define MEM_FLAG_DIRECT (1 << 2)
#define MEM_FLAG_COHERENT (2 << 2)
#define MEM_FLAG_L1_NONALLOCATING (MEM_FLAG_DIRECT | MEM_FLAG_COHERENT)

#define LEVELS_PER_TICK 50
#define BUFFER_MS 100
#define LEVEL_CNT (BUFFER_MS * (1000 / CLK_MICROS)) // Number of `level` entries in buffer
#define TICK_CNT (LEVEL_CNT / LEVELS_PER_TICK)
#define DELAY_CNT LEVEL_CNT

#define CB_CNT (LEVEL_CNT + TICK_CNT + DELAY_CNT)

#define CLK_DIVI 5
#define CLK_MICROS 1
#define SLEEP_TIME_MILLIS 5

typedef struct DMACtrlReg{
    uint32_t cs;      						// DMA Channel Control and Status register
    uint32_t cb_addr; 						// DMA Channel Control Block Address
} DMACtrlReg_t;

typedef struct DMAControlBlock{
    uint32_t tx_info;    					// Transfer information
    uint32_t src;        					// Source (bus) address
    uint32_t dest;       					// Destination (bus) address
    uint32_t tx_len;     					// Transfer length (in bytes)
    uint32_t stride;     					// 2D stride
    uint32_t next_cb;    					// Next DMAControlBlock (bus) address
    uint32_t padding[2]; 					// 2-word padding
} DMAControlBlock_t;

typedef struct DMAMemHandle{
    void *virtual_addr; 					// Virtual base address of the page
    uint32_t bus_addr;  					// Bus adress of the page, this is not a pointer because it does not point to valid virtual address
    uint32_t mb_handle; 					// Used by mailbox property interface
    uint32_t size;
} DMAMemHandle_t;

#endif // !DMA_H