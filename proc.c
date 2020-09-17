#include <linux/module.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <linux/sched.h>

MODULE_AUTHOR("CHAGS");
MODULE_DESCRIPTION("PROC ME KUCH KUCH ");

#define proc_name "ChagsProcName"

umode_t mode = S_IFREG | S_IRUGO | S_IWUSR | S_IWGRP;
struct proc_dir_entry *proc_file;

int open_file(struct inode *inode,struct file *pfile){

	printk("Entered into %s\n",__func__);
	return 0;
}

int close_file(struct inode *inode, struct file *pfile){
	printk("Entered into %s\n",__func__);
	return 0;	
}

ssize_t read_file(struct file *pfile,char *buffer, size_t length, loff_t *offset){
	printk("Entered into %s\n",__func__);
	return 0;
}

ssize_t write_file(struct file *pfile,const char *buffer, size_t length, loff_t *offset){
	printk("Entered into %s\n",__func__);
	return 0;
}

int permission_module(struct inode *inode, int operation){

	if( operation == 4 || ( operation == 2 /*&& get_current_user()->__u == 0*/ )){
		return 0;
	}

	return -EACCES;

}

struct file_operations file = {
	.owner = THIS_MODULE,
	.read  = read_file,
	.write = write_file,
	.open  = open_file,
	.release = close_file,
};

struct inode_operations inode = {
	.permission = permission_module,
};

int entry_module(void)
{
	printk("Entered into %s\n",__func__);
	proc_file=proc_create(proc_name,mode,NULL,&file);
	proc_file->iops=inode_operations;
	return 0;
}

void exit_module(void)
{
	remove_proc_entry(proc_name,NULL);
	printk("Entered into %s\n",__func__);
}

module_init(entry_module);
module_exit(exit_module);