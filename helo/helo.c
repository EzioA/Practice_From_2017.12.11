#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define DATA_MAX 256

MODULE_LICENSE("Dual BSD/GPL");

static dev_t devno;
static struct class* cls;
static struct cdev cdev;
static size_t size;
static char BUFFER[DATA_MAX];

static int helo_open(struct inode *inode, struct file *filp)
{
	printk(KERN_NOTICE "helo_open\n");
	return 0;
}

static int helo_release(struct inode *inode, struct file *filp)
{
	printk(KERN_NOTICE "helo_release\n");
	return 0;
}

static ssize_t helo_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	int ret = 0;
	int i;
	printk("count = %d\n", count);
	printk("f_pos = %lu\n", *f_pos);

	if (*f_pos >= size)
		goto out;
	if (*f_pos + count > size)
	{
		ret = size - *f_pos;
		copy_to_user(buf, &BUFFER + *f_pos, ret);
		*f_pos += ret;
		goto out;
	}
	copy_to_user(buf, &BUFFER + *f_pos, count);
	*f_pos += count;
	ret = count;
out:	printk("ret = %d\n", ret);
	for (i = 0; i < size; i++)
		printk("%c", BUFFER[i]);
	return ret;
}

static ssize_t helo_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	int ret = 0;
	size_t i;
	printk("request write count:%d\n", count);
	printk("write f_pos = %lu\n", *f_pos);

	if (count > DATA_MAX || *f_pos >= DATA_MAX)
		goto out2;
	if (*f_pos + count > DATA_MAX)
	{
		ret = DATA_MAX - *f_pos;
		copy_from_user(&BUFFER + *f_pos, buf, ret);
		*f_pos += ret;
		if (size < *f_pos)
			size = *f_pos;
		goto out2;
	}
	copy_from_user(&BUFFER + *f_pos, buf, count);
	BUFFER[*f_pos + 3] = 'c';
	printk("&BUFFER:%x\n", &BUFFER);
	printk("*f_pos:%x\n", *f_pos);
	printk("&BUFFER + *f_pos:%x\n", &BUFFER + *f_pos);
	*f_pos += count;
	ret = count;
	if (size < *f_pos)
		size = *f_pos;
out2:	printk("write count:%d\n", ret);
	printk("size:%d\n", size);
	printk("end f_pos:%d\n", *f_pos);
	for (i = 0; i < size; i++)
		printk("%c\n", BUFFER[i]);
	return ret;
}

static loff_t helo_seek(struct file *filp, loff_t off, int whence)
{
	loff_t pos;

	printk("former pos:%ld\n", filp->f_pos);
	switch(whence)
	{
		case 0:
			pos = off;
			break;
		case 1:
			pos = filp->f_pos + off;
			break;
		case 2:
			pos = size + off;
			break;
		default:
			return -EINVAL;
	}
	if (pos < 0)
		return -EINVAL;
	filp->f_pos = pos;
	printk("new pos:%ld\n", filp->f_pos);
	return pos;
}

static struct file_operations helo_ops = {
	.release = helo_release,
	.open = helo_open,
	.read = helo_read,
	.write = helo_write,
	.llseek = helo_seek
};

static int helo_init(void)
{
	alloc_chrdev_region(&devno, 0, 1, "helo");
//	cdev.owner = THIS_MODULE;
	cdev_init(&cdev, &helo_ops);
	cdev_add(&cdev, devno, 1);

	cls = class_create(THIS_MODULE, "helo");
	device_create(cls, NULL, devno, NULL, "helo");

	printk(KERN_ALERT "Success init\n");
	return 0;
}

static void helo_exit(void)
{
	device_destroy(cls, devno);
	class_destroy(cls);

	cdev_del(&cdev);
	unregister_chrdev_region(devno, 1);
	printk(KERN_ALERT "Goodbye world\n");
}

module_init(helo_init);
module_exit(helo_exit);
