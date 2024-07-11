#include "gpio.h"

volatile uint32_t *virt_gpio_regs;

int init_gpio(void){
	uint32_t periph_addr = bcm_host_get_peripheral_address();

	printf("addr gpio = %x\r\n",periph_addr + GPIO_BASE);
	virt_gpio_regs = (uint32_t*) map_segment((void *) (periph_addr + GPIO_BASE), PAGE_SIZE);
	if(!virt_gpio_regs){
		printf("erreur lors de map_segment pour GPIO\r\n");
		return 0;
	}

	return 1;
}