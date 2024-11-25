#ifndef PISMI_H
#define PISMI_H

#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/sched/signal.h>
#include <linux/ioctl.h>
#include <linux/gpio.h>

#include "../inc/gpio.h"
#include "../inc/smi.h"
#include "../inc/dma.h"

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("BOLLIAND Thomas");
MODULE_DESCRIPTION("developement d'un driver pour l'utilisation du SMI");

#define DRIVER_NAME "KpiSMI"
#define DRIVER_CLASS "KpiSMIClass"
#define SIGNR 44
#define REGISTER_UAPP _IO('S', 'g')
#define GPIO_NUMBER(a) (GPIO_DYNAMIC_BASE+a)
#define GPIO_IRQ GPIO_NUMBER(26)

static ssize_t Dread(struct file *f, char *user_buffer, size_t count, loff_t *offs);
static ssize_t Dwrite(struct file *f, const char *user_buffer, size_t count, loff_t *offs);
static int Dopen(struct inode *df, struct file *inst);
static int Dclose(struct inode *df, struct file *inst);
static long int Dioctl(struct file *file, unsigned cmd, unsigned long arg);
static int __init Dinit(void);
static void __exit Dexit(void);
module_init(Dinit);
module_exit(Dexit);

#else	// !PISMI_H
#error ne pas inclure 2 fois piSMI.h
#endif 