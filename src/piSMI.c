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
	uint32_t delta = 0;
	uint8_t tab[15] = "test de copie\n";
	delta = count - copy_to_user(user_buffer, tab, 15);
	
	return delta;
}

static ssize_t Dwrite(struct file *f, const char *user_buffer, size_t count, loff_t *offs){
	uint16_t tab[1024];
	memset(tab, '\0', sizeof(tab));
	for(uint32_t i=0; i<((count >= 1024)? 1023 : count); i++) tab[i] = user_buffer[i];

	smi_block_write(0,((count >= 1024)? 1023 : count), tab, 2, 5000);
	return (count >= 1024)? 1023 : count;
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

	if(!gpio_init()){
		printk(KERN_ERR "probleme initialisation gpio regs\r\n");
		goto gpioIniterror;
	}

	if(!smi_init()){
		printk(KERN_ERR "probleme initialisation smi\r\n");
		goto smiInitError;
	}
	if(!smi_setup()){
		printk(KERN_ERR "probleme setup du smi\r\n");
		goto smiInitError;
	}

	struct *file vcioF = filp_open("/dev/vcio", 0);
	return 0;
smiInitError :
	smi_freeAll();
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
	smi_freeAll();
	printk(KERN_NOTICE "goodbye, kernel!\r\n");
}