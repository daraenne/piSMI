obj-m := src/KpiSMI.o
# KpiSMI-obj := src/gpio.o src/smi.o src/dma.o src/mailbox.o

KERNEL_DIR := /lib/modules/$(shell uname -r)/build

all :
	make -C $(KERNEL_DIR) M=$(PWD) modules
	cp src/*.ko ./

clean :
	make -C $(KERNEL_DIR) M=$(PWD) clean
