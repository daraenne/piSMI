#include "../inc/piSMI.h"

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = Dopen,
	.release = Dclose,
	.read = Dread,
	.write = Dwrite,
	.unlocked_ioctl = Dioctl,
};

/* /dev file */
static dev_t devNr;
static struct class *devClass;
static struct cdev devFile;

/* ioctl et irq */
static struct task_struct *task = NULL;


static ssize_t Dread(struct file *f, char *user_buffer, size_t count, loff_t *offs){
	uint32_t delta = 0;
	uint8_t tab[15] = "test de copie\n";
	delta = count - copy_to_user(user_buffer, tab, 15);
	
	return delta;
}

static ssize_t Dwrite(struct file *f, const char *user_buffer, size_t count, loff_t *offs){
	uint16_t *tab = kmalloc(count * sizeof(*tab), GFP_KERNEL);
	for(uint32_t i=0; i<((count >= 1024)? 1023 : count); i++) tab[i] = user_buffer[i];
	smi_dma_write(0,((count >= 1024)? 1023 : count) * sizeof(*tab), tab, 2, 0, 20);
	kfree(tab);
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

static long int Dioctl(struct file *file, unsigned cmd, unsigned long arg) {
	if(cmd == REGISTER_UAPP) {
		task = get_current();
		printk("gpio_irq_signal: Userspace app with PID %d is registered\n", task->pid);
	}
	return 0;
}

static irq_handler_t gpio_irq_handler(unsigned int irq, void *dev_id){
	struct siginfo info;
	printk("gpio_irq_signal: Interrupt was triggered and ISR was called!\n");

	if(task != NULL) {
		memset(&info, 0, sizeof(info));
		info.si_signo = SIGNR;
		info.si_code = SI_QUEUE;

		if(send_sig_info(SIGNR, (struct kernel_siginfo *) &info, task) < 0) printk("gpio_irq_signal: Error sending signal\n");
	}
	return IRQ_HANDLED;
}

static int __init Dinit(void) {
	printk(KERN_NOTICE "Init du module pour le fonctionnement du SMI\r\n");
	
	printk(KERN_NOTICE "Creation du dev file\r\n");
	if(alloc_chrdev_region(&devNr, 0, 1, DRIVER_NAME) < 0){
		printk(KERN_ERR "Erreur : impossible d'allouer le Dev Nr\r\n");
		goto devNrError;
	}
	printk(KERN_INFO "read_write dev_nr : %d.%d\r\n", devNr >> 20, devNr & 0xfffff);
	if((devClass = class_create(DRIVER_CLASS)) == NULL){
		printk(KERN_ERR "Erreur : impossible de créer la classe\r\n");
		goto classError;
	}
	if(device_create(devClass, NULL, devNr, NULL, DRIVER_NAME) == NULL){
		printk(KERN_ERR "Erreur : impossible de créer le fichier dans /dev/\r\n");
		goto fileError;
	}
	cdev_init(&devFile, &fops);
	if(cdev_add(&devFile, devNr, 1) == 1){
		printk(KERN_ERR "Erreur : impossible d'enregistrer le module au kernel\r\n");
		goto addError;
	}
	printk(KERN_NOTICE "module enregistrer et lancer correctement, lancement des initialisations périphériques\r\n");
	if(!gpio_init()){
		printk(KERN_ERR "Erreur : impossible de récupérer les registres GPIO\r\n");
		goto gpioIniterror;
	}
	printk(KERN_NOTICE "Initialisation des gpio OK\r\n");
	if(!dma_init()){
		printk(KERN_ERR "Erreur : impossible de récupérer les registres DMA et d'allouer les CBs\r\n");
		goto dmaInitError;
	}
	printk(KERN_NOTICE "Initialisation des DMA OK\r\n");
	if(!smi_init()){
		printk(KERN_ERR "Erreur : impossible de récupérer les registres SMI et d'allouer les buffer pour DMA\r\n");
		goto smiInitError;
	}
	if(!smi_setup()){
		printk(KERN_ERR "Erreur : impossible de setup le SMI\r\n");
		goto smiInitError;
	}
	// printk(KERN_NOTICE "Initialisation et setup du SMI OK\r\n");
	// if(gpio_request(BUTTON, "bouton")) {
	// 	printk("Error!\nCan not allocate bouton (gpio %d)\n", BUTTON - GPIO_DYNAMIC_BASE);
	// 	goto addError;
	// }
	// if(gpio_direction_input(BUTTON)) {
	// 	printk("Error!\nCan not set bouton to input!\n");
	// 	goto gpioDirError;
	// }
	// irq_number = gpio_to_irq(BUTTON);
	// if(request_irq(irq_number, (irq_handler_t) gpio_irq_handler, IRQF_TRIGGER_RISING, "my_gpio_irq_signal", NULL) != 0){
	// 	printk("Error!\nCan not request interrupt nr.: %d\n", irq_number);
	// 	goto gpioDirError;
	// }
	// printk("button is mapped to IRQ Nr.: %d\n", irq_number);
	return 0;
smiInitError:
	smi_freeAll();
dmaInitError:
	dma_freeAll();
gpioIniterror:
	gpio_freeAll();
	cdev_del(&devFile);
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
	smi_freeAll();
	dma_freeAll();
	gpio_freeAll();
	cdev_del(&devFile);
	device_destroy(devClass, devNr);
	class_destroy(devClass);
	unregister_chrdev(devNr, DRIVER_NAME);
	printk(KERN_NOTICE "goodbye, kernel!\r\n");
}