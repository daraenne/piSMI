#ifndef KPISMI_H
#define KPISMI_H

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("BOLLIAND Thomas");
MODULE_DESCRIPTION("developement d'un driver pour l'utilisation du SMI");

#define DRIVER_NAME "KpiSMI"
#define DRIVER_CLASS "KpiSMIClass"

static ssize_t Dread(struct file *f, char *user_buffer, size_t count, loff_t *offs);
static ssize_t Dwrite(struct file *f, const char *user_buffer, size_t count, loff_t *offs);
static int Dopen(struct inode *df, struct file *inst);
static int Dclose(struct inode *df, struct file *inst);
static int __init Dinit(void);
static void __exit Dexit(void);
module_init(Dinit);
module_exit(Dexit);

#endif