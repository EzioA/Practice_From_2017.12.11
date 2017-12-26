#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define DATA_MAX 256

MODULE_LICENSE("Dual BSD/GPL");

//static int major = 50;
//static int minor = 0;
static char BUFFER[DATA_MAX];
static dev_t devno;
//static struct cdev cdev;
static struct class* cls;

static struct mydev
{
	unsigned int size;
	struct cdev cdev;
} dev = {
	.size = 0
};


static int hello_open(struct inode *inode, struct file *filep)
{
//	struct mydev *dev;
//	dev = container_of(inode->i_cdev, struct mydev, cdev);
	filep->private_data = &dev;
	printk(KERN_NOTICE "hello_open");

	return 0;
}

static int hello_release(struct inode *inode, struct file *filep)
{
	printk(KERN_NOTICE "hello_release");

	return 0;
}

static ssize_t hello_read(struct file *filep, char *buf, size_t count, loff_t *off_p)
{
	struct mydev *dev = filep->private_data;
	int ret = 0;
	printk("count = %d", count);
	printk("off_p = %lu", *off_p);
//	printk("size of file:%d", dev->size);
	if (*off_p >= dev->size)
		return ret;
	if (*off_p + count > dev->size)
	{
		ret = dev->size - *off_p;
	//	printk("ret = %d", ret);
		copy_to_user(buf, &BUFFER + *off_p, ret);
		*off_p += ret;
		return ret;
	}
	copy_to_user(buf, &BUFFER + *off_p, count);
	*off_p += count;
	ret = count;

	return ret;
}

static ssize_t hello_write(struct file *filep, const char *buf, size_t count, loff_t *off_p)
{
	struct mydev *dev = filep->private_data;
	unsigned long ret = 0;
	printk("request write count:%d", count);
	if (count >= DATA_MAX || *off_p >= DATA_MAX)
		return ret;
	if (*off_p + count > DATA_MAX)
	{
		ret = DATA_MAX - *off_p;
		copy_from_user(&BUFFER + *off_p, buf, ret);
		*off_p += ret;
		if (dev->size < *off_p)
			dev->size = *off_p;
		return ret;
	}
	copy_from_user(&BUFFER + *off_p, buf, count);
	*off_p += count;
	if (dev->size < *off_p)
		dev->size = *off_p;
	ret = count;

	return ret;
}

static loff_t hello_seek(struct file *filep, loff_t off, int whence)
{
	struct mydev *dev = filep->private_data;
	loff_t pos;

	switch (whence)
	{
		case 0:
			pos = off;
			break;
		case 1:
			pos = filep->f_pos + off;
			break;
		case 2:
			pos = dev->size + off;
			break;
		default:
			return -EINVAL;
	}
	if (pos < 0)
		return -EINVAL;
	filep->f_pos = pos;
	return pos;
}


static struct file_operations hello_ops = 
{
	.open = hello_open,
	.release = hello_release,
	.read = hello_read,
	.write = hello_write,
	.llseek = hello_seek
};

static int hello_init(void)
{
	alloc_chrdev_region(&devno, 0, 1, "hello");

	dev.cdev.owner = THIS_MODULE;
	cdev_init(&dev.cdev, &hello_ops);
	cdev_add(&dev.cdev, devno, 1);

	cls = class_create(THIS_MODULE, "hello");
	device_create(cls, NULL, devno, NULL, "hello");

	printk(KERN_ALERT "Success init hello");
	return 0;
}

static void hello_exit(void)
{
	device_destroy(cls, devno);
	class_destroy(cls);

	cdev_del(&dev.cdev);
	unregister_chrdev_region(devno, 1);
	printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
