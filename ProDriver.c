//Header for module
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>

//whoami
MODULE_AUTHOR("Chags");
MODULE_DESCRIPTION("This is my first professional module");
MODULE_LICENSE("GPL");

//Declaring functions that will be defined later in program
static int entry_module(void);
static void exit_module(void);
static int file_open(struct inode *pinode,struct file *pfile);
static int file_release(struct inode *pinode, struct file *pfile);
static ssize_t file_module_read(struct file *pfile,char *buffer,size_t length,loff_t *offset);
static ssize_t file_module_write(struct file *pfile ,const char *buffer, size_t length , loff_t *pffset);
static long device_ioctl(struct file *file, unsigned int ioctl_num,unsigned long ioctl_param);
//Defining variables
#define driver_name "ProDriver"

#define MAJOR_NUM 100
#define IOCTL_SET_MSG _IOW(MAJOR_NUM, 0, char *)
#define IOCTL_GET_MSG _IOR(MAJOR_NUM, 1, char *)
#define IOCTL_GET_NTH_BYTE _IOWR(MAJOR_NUM, 2, int)


static int rc;						//Return code for functions
static int major_number;			//Will save dynamically assigned major number ky kernel
static int semaphore;
static int val;
/*
	Creating struct object that will point to out module functions for file operations
*/
static const struct file_operations function_info = {
	.owner   = 			THIS_MODULE,
	.read    = 			file_module_read,
	.write   = 			file_module_write,
	.open  	 = 			file_open,
	.unlocked_ioctl = 	device_ioctl,
	.release = 			file_release,

};

//Called when file is opened
static int file_open(struct inode *pinode,		//Inode of file
					struct file *pfile)			//File pointer
{
	printk(KERN_INFO "File is involved %s is called \n",__func__);
	if(semaphore != 0)
	{
		printk(KERN_ERR "File already in use\n");
		//sprintf("eh-err file already in use\n");
		return -1;
	}
	semaphore++;
	return 0;
}

//Called when file is released
static int file_release(struct inode *pinode,	//Inode of file
						struct file *pfile)		//File pointer
{
	printk(KERN_INFO "File is involved %s is called \n",__func__);
	semaphore--;
	return 0;
}

//Called when data is getting read from file
static ssize_t file_module_read(struct file *pfile,	//File pointer
						 char *buffer,	//Buffer at user end i.e what user wants to read
						 size_t length,			//Length of data that user want to read
						 loff_t *offset)		//Offset in data where operation is getting formed
{
	printk(KERN_INFO "File is involved %s is called \n",__func__);
	char *val1="321";
	copy_to_user(buffer,val1,1024);
	printk(KERN_INFO "data is %s",buffer);


	return 1024;
}

//Called when data is getting appended into file 
static ssize_t file_module_write(struct file *pfile,	//File pointer
						  const char *buffer,			//Buffer at user end i.e what user wants to write
						  size_t length,		//Length of data that user want to write
						  loff_t *offset)		//Offset in data where operation is getting formed
{
	printk(KERN_INFO "File is involved %s is called \n",__func__);
	printk(KERN_INFO "length = %ld,ofset = %s",length,buffer);
	return length;
}

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

//Called when module is loaded
static int entry_module(void)
{
	printk(KERN_INFO "Entered into :%s \n",__func__);
	printk(KERN_INFO "Trying to register the charcter device \n");
	/*
	registering device
	*/
	rc=register_chrdev(0,driver_name,&function_info);

	if(rc<0)
	{
		printk(KERN_ERR "Error: unable to register device with rc =%d\n",rc);
		return rc;
	}

	//Saving major number from rc if not < 0
	major_number=rc;
	printk("This charcter device %s was loaded sucessfully and has been assigned %d major number\n",driver_name,major_number);

	//Making semaphore 0 in order to achieve file locking
	semaphore=0;
	return 0;
}

//Called when module is removed from kernel
static void exit_module(void)
{
	printk(KERN_INFO "Entered into :%s \n",__func__);
	printk(KERN_INFO "Trying to de-register the charcter device \n");

	unregister_chrdev(major_number,driver_name);
	// if(rc<0)
	// {
	// 	printk(KERN_ERR "Unable to un register the charcter device from the kernel\n Please look into it on priorrity\n" );
		
	// }else
	// {
		printk(KERN_INFO "Character device was sucesfully inregistered from the kernel");
	//}
} 


//Macros to declare which  initialize and exit function should be used  
module_init(entry_module);
module_exit(exit_module);