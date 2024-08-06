#ifndef GPIO_H
#define GPIO_H

#include <linux/bits.h>
#include <linux/io.h>

#define PERIPH_BASE				0xFE000000
#define GPIO_BASE				0x200000
#define GPIO_REGS_SIZE			0xF0
#define GPIO_FuncSel_0			0x00		// 0x00/4
#define GPIO_FuncSel_1			0x01		// 0x04/4
#define GPIO_SET_0				0x07		// 0x1c/4
#define GPIO_CLR_0				0x0A		// 0x28/4
#define GPIO_LEV_0				0x0D		// 0x34/4

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) 		*(virt_gpio_regs+((g)/10)) 		&= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) 		*(virt_gpio_regs+((g)/10)) 		|=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) 	*(virt_gpio_regs+(((g)/10))) 	|= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

typedef enum{alt0=0, alt1=1, alt2=2, alt3=3, alt4=4, alt5=5, IN=6, OUT=7}gpio_func_t;

uint8_t gpio_init(void);
void gpio_freeAll(void);
uint8_t gpio_config(uint8_t gpio, gpio_func_t func);
uint8_t gpio_setVal(uint8_t gpio, uint8_t val);

#endif // !GPIO_H