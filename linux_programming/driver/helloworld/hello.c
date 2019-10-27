#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/slab.h>	
#include <linux/kdev_t.h>


MODULE_LICENSE("GPL");

static char *whom = "world";
static int howmany = 1;
static int hello_major;
static int  hello_minor = 0;
char *module_name = "hello";

module_param(howmany, int, S_IRUGO);
module_param(whom, charp, S_IRUGO);


struct cdev *cdev;
dev_t dev_num;

struct file_operations f_ops = {

};


static int __init hello_init(void)
{
	int i;
	int ret;
	

//	printk(KERN_ALERT "hello world!\n");

/*	printk(KERN_ALERT "cnt = %d\n", howmany);
	for (i= 0; i< howmany; ++i) {
		printk(KERN_ALERT "hello, %s!\n", whom);
	}
*/

	if (hello_major) {
		register_chrdev_region(dev_num, 1, module_name);
	} else {
		ret = alloc_chrdev_region(&dev_num, hello_minor, 1, module_name);
		if (ret < 0) {
			printk("alloc chrdev region failed!\n");
			return -1;
		}
	}

	cdev = cdev_alloc();
	cdev->ops = &f_ops;
	cdev->owner = THIS_MODULE;

	ret = cdev_add(cdev, dev_num, 1);
	if (ret < 0) {
		printk("cdev add hello failed!\n");
		unregister_chrdev_region(dev_num, 1);
		return -1;
	}
	printk("init hello success, major = %d\n", MAJOR(dev_num));

	return 0;
}

static void __exit hello_exit(void)
{
	cdev_del(cdev);
	unregister_chrdev_region(dev_num, 1);

	printk(KERN_ALERT "hello exit\n");
}




module_init(hello_init);
module_exit(hello_exit);

