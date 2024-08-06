obj-m := bin/KpiSMI.o
bin/KpiSMI-objs := src/piSMI.o src/gpio.o #src/smi.o src/dma.o src/mailbox.o

KERNEL_DIR := /lib/modules/$(shell uname -r)/build

all :
	make -C $(KERNEL_DIR) M=$(PWD) modules
	cp bin/*.ko ./

clean :
	make -C $(KERNEL_DIR) M=$(PWD) clean
