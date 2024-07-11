#include "dma.h"

volatile uint32_t *virt_dma_regs;
// int mailbox_fd = -1;
// DMAMemHandle_t *dma_cbs;
// DMAMemHandle_t *dma_ticks;
// DMAMemHandle_t *dma_levels;
// volatile DMACtrlReg_t *dma_reg;

// DMAMemHandle_t *dma_malloc(unsigned int size){
//     if (mailbox_fd < 0){
//         mailbox_fd = mbox_open();
//         assert(mailbox_fd >= 0);
//     }

//     // Make `size` a multiple of PAGE_SIZE
//     size = ((size + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;

//     DMAMemHandle_t *mem = (DMAMemHandle_t *)malloc(sizeof(DMAMemHandle_t));
//     // Documentation: https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface
//     mem->mb_handle = mem_alloc(mailbox_fd, size, PAGE_SIZE, MEM_FLAG_L1_NONALLOCATING);
//     mem->bus_addr = mem_lock(mailbox_fd, mem->mb_handle);
//     mem->virtual_addr = mapmem(BUS_TO_PHYS(mem->bus_addr), size);
//     mem->size = size;

//     assert(mem->bus_addr != 0);

//     fprintf(stderr, "MBox alloc: %d bytes, bus: %08X, virt: %08X\n", mem->size, mem->bus_addr, (uint32_t)mem->virtual_addr);

//     return mem;
// }

// void dma_free(DMAMemHandle_t *mem){
//     if (mem->virtual_addr == NULL) return;

//     unmapmem(mem->virtual_addr, PAGE_SIZE);
//     mem_unlock(mailbox_fd, mem->mb_handle);
//     mem_free(mailbox_fd, mem->mb_handle);
//     mem->virtual_addr = NULL;
// }