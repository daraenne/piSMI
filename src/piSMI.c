#include "../inc/piSMI.h"

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = Dopen,
	.release = Dclose,
	.read = Dread,
	.write = Dwrite
};

/* /dev file */
static dev_t devNr;
static struct class *devClass;
static struct cdev devFile;

uint8_t lastvalgpio;

static ssize_t Dread(struct file *f, char *user_buffer, size_t count, loff_t *offs){
	memcpy(user_buffer, "test de copie", 14);
	return count;
}

static ssize_t Dwrite(struct file *f, const char *user_buffer, size_t count, loff_t *offs){
	uint8_t tab[1024];
	memset(tab, '\0', 1024);
	memcpy(tab, user_buffer,(count > 1024)? 1023 : count);
	printk("u wrote %s\r\n", tab);

	lastvalgpio ^= 1;
	gpio_setVal(21, lastvalgpio);

	return (count > 1024)? 1023 : count;
}

static int Dopen(struct inode *df, struct file *inst){
	printk("file opened!\r\n");
	return 0;
}

static int Dclose(struct inode *df, struct file *inst){
	printk("file closed!\r\n");
	return 0;
}

static int __init Dinit(void) {
	printk(KERN_NOTICE "hello, kernel!\r\n");
	
	/* alloc dev nr */
	if(alloc_chrdev_region(&devNr, 0, 1, DRIVER_NAME) < 0){
		printk(KERN_ERR "fail allocate device nr");
		goto devNrError;
	}
	printk(KERN_INFO "read_write dev_nr : %d.%d\r\n", devNr >> 20, devNr & 0xfffff);

	/* create dev class */
	if((devClass = class_create(DRIVER_CLASS)) == NULL){
		printk(KERN_ERR "fail to create class\r\n");
		goto classError;
	}

	/* create dev file */
	if(device_create(devClass, NULL, devNr, NULL, DRIVER_NAME) == NULL){
		printk(KERN_ERR "fail to create file\r\n");
		goto fileError;
	}

	/* initialize dev file */
	cdev_init(&devFile, &fops);

	/* registering device to kernel */
	if(cdev_add(&devFile, devNr, 1) == 1){
		printk(KERN_ERR "fail to register dev file to kernel\r\n");
		goto addError;
	}

	if(gpio_init() < 0){
		printk(KERN_ERR "probleme initialisation gpio regs\r\n");
		goto gpioIniterror;
	}

	if(!gpio_config(21, OUT)){
		printk(KERN_ERR "probleme configuration gpio 21");
		goto gpioIniterror;
	}
	lastvalgpio = 0;

	return 0;
gpioIniterror :
	gpio_freeAll();
addError:
	device_destroy(devClass, devNr);
fileError:
	class_destroy(devClass);
classError:
	unregister_chrdev(devNr, DRIVER_NAME);
devNrError:
	return -1;
}

static void __exit Dexit(void){
	cdev_del(&devFile);
	device_destroy(devClass, devNr);
	class_destroy(devClass);
	unregister_chrdev(devNr, DRIVER_NAME);
	gpio_freeAll();
	printk("goodbye, kernel!\r\n");
}