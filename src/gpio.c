#include "../inc/gpio.h"

volatile unsigned *virt_gpio_regs;

uint8_t gpio_init(void){
	printk(KERN_DEBUG "addr gpio = %x\r\n",PERIPH_BASE + GPIO_BASE);
	virt_gpio_regs = (uint32_t*) ioremap(PERIPH_BASE + GPIO_BASE, GPIO_REGS_SIZE);
	if(!virt_gpio_regs){
		printk(KERN_ERR "erreur lors de map_segment pour GPIO\r\n");
		return 0;
	}
	return 1;
}

void gpio_freeAll(void){
	if(!virt_gpio_regs) iounmap(virt_gpio_regs);
}

uint8_t gpio_config(uint8_t gpio, gpio_func_t func){
	if(!virt_gpio_regs){
		printk(KERN_ERR "la fonction gpio init n'a pas ete appele ou avait echouer\r\n");
		return 0;
	}

	INP_GPIO(gpio);
	switch(func){
		case IN: 	break;
		case OUT: 	OUT_GPIO(gpio); break;
		default:	SET_GPIO_ALT(gpio, func);
	}
	return 1;
}

uint8_t gpio_setVal(uint8_t gpio, uint8_t val){
	if(!virt_gpio_regs){
		printk(KERN_ERR "la fonction gpio init n'a pas ete appele ou avait echouer\r\n");
		return 0;
	}

	if(val) *(virt_gpio_regs + GPIO_SET_0) = 1<<gpio;
	else 	*(virt_gpio_regs + GPIO_CLR_0) = 1<<gpio;

	return 1;
}