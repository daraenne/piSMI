#ifndef GPIO_H
#define GPIO_H

#include "main.h"

#define GPIO_BASE				0x200000
#define GPIO_FuncSel_0			0x00		// 0x00/4
#define GPIO_FuncSel_1			0x01		// 0x04/4
#define GPIO_SET_0				0x07		// 0x1c/4
#define GPIO_CLR_0				0x0A		// 0x28/4
#define GPIO_LEV_0				0x0D		// 0x34/4

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) 		*(virt_gpio_regs+((g)/10)) 		&= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) 		*(virt_gpio_regs+((g)/10)) 		|=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) 	*(virt_gpio_regs+(((g)/10))) 	|= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

int init_gpio(void);
void testGpio(void);

#endif // !GPIO_H