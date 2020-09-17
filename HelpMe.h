<------------------------------------------------------------------------->
#Basic init/cleanup macros

module_init(initialize_character_module1);
module_exit(destroying_character_module);

<------------------------------------------------------------------------->

KERN_EMERG - n = 0
KERN_ALERT - n = 1
KERN_CRIT - n = 2
KERN_ERR - n = 3
KERN_WARNING - n = 4
KERN_NOTICE - n = 5
KERN_INFO - n = 6
KERN_DEBUG - n = 7

<------------------------------------------------------------------------->

pr_emerg(fmt, ...); /* similar to printk(KERN_EMERG pr_fmt(fmt), ...); */
pr_alert(fmt, ...); /* similar to printk(KERN_ALERT pr_fmt(fmt), ...); */
pr_crit(fmt, ...); /* similar to printk(KERN_CRIT pr_fmt(fmt), ...); */
pr_err(fmt, ...); /* similar to printk(KERN_ERR pr_fmt(fmt), ...); */
pr_warning(fmt, ...); /* similar to printk(KERN_WARNING pr_fmt(fmt), ...); */
pr_warn(fmt, ...); /* similar to cu printk(KERN_WARNING pr_fmt(fmt), ...); */
pr_notice(fmt, ...); /* similar to printk(KERN_NOTICE pr_fmt(fmt), ...); */
pr_info(fmt, ...); /* similar to printk(KERN_INFO pr_fmt(fmt), ...); */


<------------------------------------------------------------------------->
#Module parameters

module_param(number,int,0774);
//MODULE_PARAM_DESC(number, "An integer number");
<------------------------------------------------------------------------->

#FS operations
static int file_open(struct inode *pinode,struct file *pfile);
static int file_release(struct inode *pinode, struct file *pfile);
static ssize_t file_module_read(struct file *pfile,char *buffer,size_t length,loff_t *offset);
static ssize_t file_module_write(struct file *pfile ,const char *buffer, size_t length , loff_t *pffset);

#struct 
static const struct file_operations function_info = {
	.owner   = THIS_MODULE,
	.read    = file_module_read,
	.write   = file_module_write,
	.open  	 = file_open,
	.release = file_release,

};

#Add chrdev
register_chrdev(0,driver_name,&function_info);	
#Remove chrdev
unregister_chrdev(major_number,driver_name);
<------------------------------------------------------------------------->

#Inode operations

int permission_module(struct inode *inode, int operation)


<------------------------------------------------------------------------->

#Increase module use counter so that it cannot be removed with rmmod

#increment counter
try_module_get(THIS_MODULE);
#decrement counter
module_put(THIS_MODULE);

<------------------------------------------------------------------------->
#Permission

#define 	S_IRWXUGO   (S_IRWXU|S_IRWXG|S_IRWXO)
 
#define 	S_IALLUGO   (S_ISUID|S_ISGID|S_ISVTX|S_IRWXUGO)
 
#define 	S_IRUGO   (S_IRUSR|S_IRGRP|S_IROTH)
 
#define 	S_IWUGO   (S_IWUSR|S_IWGRP|S_IWOTH)
 
#define 	S_IXUGO   (S_IXUSR|S_IXGRP|S_IXOTH)

<------------------------------------------------------------------------->

#proc filesystem
#include <linux/proc_fs.h>

umode_t mode = S_IFREG | S_IRUGO | S_IWUSR;
proc_file=proc_create("PROC_NAME",mode,NULL,&file_operations);
proc_file->iops=inode_operations;
remove_proc_entry(proc_name,NULL);

<------------------------------------------------------------------------->

#IOCTL 

#theory
If you are adding new ioctl’s to the kernel, you should use the _IO macros defined in <linux/ioctl.h>:

    _IO 	an 	ioctl with no parameters
    _IOW 	an 	ioctl with write parameters (copy_from_user)
    _IOR 	an 	ioctl with read parameters (copy_to_user)
    _IOWR 	an 	ioctl with both write and read parameters.


#practical

#include <linux/uaccess.h>
#define MAJOR_NUM 100
#define IOCTL_SET_MSG _IOW(MAJOR_NUM, 0, char *)
#define IOCTL_GET_MSG _IOR(MAJOR_NUM, 1, char *)
#define IOCTL_GET_NTH_BYTE _IOWR(MAJOR_NUM, 2, int)

static const struct file_operations function_info = {
	.unlocked_ioctl = 	device_ioctl,

};

static long device_ioctl(struct file *file, unsigned int ioctl_num,unsigned long ioctl_param){


	printk(KERN_INFO "IOCTL called : with  %d code",ioctl_num);
	switch(ioctl_num)
	{
		case IOCTL_SET_MSG:
			printk(KERN_INFO "IOCTL set message\n");
			copy_from_user(&val,(int32_t*)ioctl_param,sizeof(val));
			printk(KERN_INFO  "calue i got is %d n",val);
			break;
		case IOCTL_GET_MSG:
			printk(KERN_INFO "IOCTL get message\n");
			val=321;
			copy_to_user((int32_t*)ioctl_param,&val,sizeof(val));
			break;
		case IOCTL_GET_NTH_BYTE:
			printk(KERN_INFO "IOCTL get nth message\n");
			break;	
	}
	return 0;
	
}

<------------------------------------------------------------------------->

#Denying a process read access and making it wait

#declare wait queue
DECLARE_WAIT_QUEUE_HEAD(WaitQ);

#for sleeping
while(semaphore){
		wait_event_interruptible(WaitQ, !semaphore);

		if ((file->f_flags & O_NONBLOCK) && semaphore !=0)
		return -EAGAIN;
}

#for waking up
wake_up(&WaitQ);

<------------------------------------------------------------------------->

#Replacing printk

struct tty_struct *my_tty;
	my_tty = get_current_tty();

	if(my_tty!= NULL)
	{
		(my_tty->driver->ops->write) (my_tty, ptr, strlen(ptr));
		(my_tty->driver->ops->write) (my_tty, "\015\012", 2);
	}
<------------------------------------------------------------------------->

#Scheduling task 

#declaring queue and delayed work
static struct workqueue_struct *queue;
static struct delayed_work work;

#Initializing them 
queue=create_workqueue(queue_name);
INIT_DELAYED_WORK(&work,task_function_chags);

#calling depayed work
queue_delayed_work(queue,&work,delay);

#destroying 
cancel_delayed_work(&work);
flush_workqueue(queue);
destroy_workqueue(queue);

<------------------------------------------------------------------------->

#Interrupt handlers

