#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>

MODULE_AUTHOR("Chags");
MODULE_DESCRIPTION("This is test module for character device");
MODULE_LICENSE("GPL");

int file_open(struct inode *pinode, struct file *pfile){

	printk(KERN_INFO "We are inside %s\n",__func__);
	int i=try_module_get(THIS_MODULE);
	printk(KERN_INFO "count of module get is %d\n",i);
	return 0;
}

ssize_t file_read(struct file *pfile,char __user *buffer,size_t length , loff_t *offset){
	printk(KERN_INFO "We are inside %s\n",__func__);
	return 0;
}

ssize_t file_write(struct file *pfile,const char __user *buffer,size_t length , loff_t *offset){
	printk(KERN_INFO "We are inside %s\n",__func__);
	printk(KERN_INFO "This was written %ld\n",length);
	return length;
}

int file_release(struct inode *pinode, struct file *pfile){
	printk(KERN_INFO "We are inside %s\n",__func__);
	module_put(THIS_MODULE);
	return 0;
}


struct file_operations CDriver_file_operations = {
	.owner   =THIS_MODULE,
	.open    =file_open,
	.read    =file_read,
	.write   =file_write,
	.release =file_release,
};

static int i;

int initialize_character_module1(void)
{
	printk(KERN_INFO "We are inside %s\n",__func__);
	i=register_chrdev(0,"CDeviceDriver",&CDriver_file_operations);
	printk("%d is the device number\n",i );
	return 0;
}

void destroying_character_module(void)
{
	printk(KERN_INFO "We are inside %s\n",__func__);
	unregister_chrdev(i,"CDeviceDriver");
}


module_init(initialize_character_module1);
module_exit(destroying_character_module);