#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

#define DEVICE_NAME "procshield"

static dev_t deviceNumber;
static struct cdev procshield_cdev;
static struct class *procshield_class;

static char message[256] = "ProcShield driver is running.\n";

static DEFINE_MUTEX(procshield_mutex);

static char *procshield_devnode(const struct device *dev, umode_t *mode)
{
    if (mode != NULL)
        *mode = 0666;

    return NULL;
}

static int procshield_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "ProcShield: device opened\n");
    return 0;
}

static int procshield_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "ProcShield: device closed\n");
    return 0;
}

static ssize_t procshield_read(
    struct file *file,
    char __user *buffer,
    size_t length,
    loff_t *offset)
{
    int messageLength;

    if (*offset > 0)
        return 0;

    mutex_lock(&procshield_mutex);

    messageLength = strlen(message);

    if (length < messageLength)
    {
        mutex_unlock(&procshield_mutex);
        return -EINVAL;
    }

    if (copy_to_user(buffer, message, messageLength))
    {
        mutex_unlock(&procshield_mutex);
        return -EFAULT;
    }

    *offset = messageLength;

    mutex_unlock(&procshield_mutex);

    return messageLength;
}

static ssize_t procshield_write(
    struct file *file,
    const char __user *buffer,
    size_t length,
    loff_t *offset)
{
    size_t copyLength = length;

    if (copyLength >= sizeof(message))
        copyLength = sizeof(message) - 1;

    mutex_lock(&procshield_mutex);

    memset(message, 0, sizeof(message));

    if (copy_from_user(message, buffer, copyLength))
    {
        mutex_unlock(&procshield_mutex);
        return -EFAULT;
    }

    message[copyLength] = '\0';

    mutex_unlock(&procshield_mutex);

    printk(KERN_INFO
           "ProcShield: message received from user space: %s\n",
           message);

    return copyLength;
}

static struct file_operations procshield_fops =
{
    .owner = THIS_MODULE,
    .open = procshield_open,
    .release = procshield_release,
    .read = procshield_read,
    .write = procshield_write
};

static int __init procshield_init(void)
{
    int result;

    printk(KERN_INFO "ProcShield: driver loading\n");

    result = alloc_chrdev_region(&deviceNumber, 0, 1, DEVICE_NAME);

    if (result < 0)
    {
        printk(KERN_ERR
               "ProcShield: failed to allocate device number\n");
        return result;
    }

    cdev_init(&procshield_cdev, &procshield_fops);

    result = cdev_add(&procshield_cdev, deviceNumber, 1);

    if (result < 0)
    {
        unregister_chrdev_region(deviceNumber, 1);
        printk(KERN_ERR
               "ProcShield: failed to add character device\n");
        return result;
    }

    procshield_class = class_create(DEVICE_NAME);

    if (IS_ERR(procshield_class))
    {
        cdev_del(&procshield_cdev);
        unregister_chrdev_region(deviceNumber, 1);

        printk(KERN_ERR
               "ProcShield: failed to create device class\n");

        return PTR_ERR(procshield_class);
    }

    procshield_class->devnode = procshield_devnode;

    if (IS_ERR(device_create(
            procshield_class,
            NULL,
            deviceNumber,
            NULL,
            DEVICE_NAME)))
    {
        class_destroy(procshield_class);
        cdev_del(&procshield_cdev);
        unregister_chrdev_region(deviceNumber, 1);

        printk(KERN_ERR
               "ProcShield: failed to create device\n");

        return -1;
    }

    printk(KERN_INFO "ProcShield: driver loaded successfully\n");
    printk(KERN_INFO "ProcShield: device /dev/procshield created\n");

    return 0;
}

static void __exit procshield_exit(void)
{
    device_destroy(procshield_class, deviceNumber);
    class_destroy(procshield_class);
    cdev_del(&procshield_cdev);
    unregister_chrdev_region(deviceNumber, 1);

    printk(KERN_INFO "ProcShield: driver unloaded\n");
}

module_init(procshield_init);
module_exit(procshield_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Supriti Mishra");
MODULE_DESCRIPTION("ProcShield virtual character device driver");
MODULE_VERSION("1.0");