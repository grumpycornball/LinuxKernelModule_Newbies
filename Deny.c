#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/tty.h>

MODULE_AUTHOR("Chags");
MODULE_DESCRIPTION("Denying it");
MODULE_LICENSE("GPL");

static int semaphore;
DECLARE_WAIT_QUEUE_HEAD(WaitQ);
static struct tty_struct *my_tty;
	

static int file_open(struct inode *inode,struct file *file ){
	printk(KERN_INFO "Entering %s\n",__func__);

	while(semaphore){

		if ((file->f_flags & O_NONBLOCK) && semaphore !=0)
		return -EAGAIN;
	
		wait_event_interruptible(WaitQ, !semaphore);
	}

	if(semaphore==0)
	{
		semaphore++;
		return 0;
	}else{
		return -EACCES;
	}
	
}

static int file_close(struct inode *inode, struct file *file){
	printk(KERN_INFO "Entering %s\n",__func__);
	semaphore--;
	wake_up(&WaitQ);
	if(semaphore < 0){
		printk(KERN_INFO "Dude something is wrong semaphore was %d",semaphore);
		semaphore=0;
	}
	return 0;
}

static ssize_t file_read(struct file *file, char *buffer, size_t length , loff_t *offset ){
	printk(KERN_INFO "Entering %s\n",__func__);
	char *val="You've got it";
	copy_to_user(buffer,val,12);
	return 1;
}

static ssize_t file_write(struct file *file, const char *buffer, size_t length , loff_t *offset ){
	
	printk(KERN_INFO "Entering %s\n",__func__);
	return length;
}

static struct file_operations file_opera = {
	.owner 	 = THIS_MODULE,
	.open 	 = file_open,
	.read	 = file_read,
	.write	 = file_write,
	.release = file_close,
};


void print_string(char *ptr){

	
	if(my_tty!= NULL)
	{
		(my_tty->driver->ops->write) (my_tty, ptr, strlen(ptr));
		(my_tty->driver->ops->write) (my_tty, "\015\012", 2);
	}
}

static int initalize_module(void)
{
	semaphore=0;
	my_tty = get_current_tty();
	printk(KERN_INFO "Entering %s\n",__func__);
	print_string("Entering the module chags chaggy");
	umode_t mode;
	mode = S_IFREG | S_IRUGO | S_IWUGO;
	proc_create("Chags",mode,NULL,&file_opera);
	return 0;
}

static void exiting_module(void){

	remove_proc_entry("Chags",NULL);
	printk(KERN_INFO "Entering %s\n",__func__);
	print_string("Existing the module chags chaggy");
}

module_init(initalize_module);
module_exit(exiting_module);