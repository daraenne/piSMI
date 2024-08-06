#include "../inc/KpiSMI.h"

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

static ssize_t Dread(struct file *f, char *user_buffer, size_t count, loff_t *offs){
	memcpy(user_buffer, "test de copie", 14);
	return count;
}

static ssize_t Dwrite(struct file *f, const char *user_buffer, size_t count, loff_t *offs){
	uint8_t tab[1024];
	memset(tab, '\0', 1024);
	memcpy(tab, user_buffer,(count > 1024)? 1023 : count);
	printk("u wrote %s\r\n", tab);
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
	printk("hello, kernel!\r\n");
	
	/* alloc dev nr */
	if(alloc_chrdev_region(&devNr, 0, 1, DRIVER_NAME) < 0){
		printk("fail allocate device nr");
		goto devNrError;
	}
	printk("read_write dev_nr : %d.%d\r\n", devNr >> 20, devNr & 0xfffff);

	/* create dev class */
	if((devClass = class_create(DRIVER_CLASS)) == NULL){
		printk("fail to create class\r\n");
		goto classError;
	}

	/* create dev file */
	if(device_create(devClass, NULL, devNr, NULL, DRIVER_NAME) == NULL){
		printk("fail to create file\r\n");
		goto fileError;
	}

	/* initialize dev file */
	cdev_init(&devFile, &fops);

	/* registering device to kernel */
	if(cdev_add(&devFile, devNr, 1) == 1){
		printk("fail to register dev file to kernel\r\n");
		goto addError;
	}

	return 0;

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
	printk("goodbye, kernel!\r\n");
}