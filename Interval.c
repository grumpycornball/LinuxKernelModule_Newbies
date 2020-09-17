#include <linux/module.h>
//#include <linux/kernel.h>
#include <linux/workqueue.h>
#include <linux/sched.h>

MODULE_AUTHOR("Chags");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Self calling babes");

#define queue_name "ChaggyQueue"

static int times_called,terminate;

static struct workqueue_struct *queue;
static struct delayed_work work;

static int delay=5000;

static void task_function_chags(struct work_struct *workky){

	times_called++;
	printk(KERN_INFO "Into task functino %d time \n",times_called );
	if(terminate == 0)
		queue_delayed_work(queue,&work,delay);
}

int model_initialize(void){
	times_called=0;
	terminate=0;
	queue=create_workqueue(queue_name);
	INIT_DELAYED_WORK(&work,task_function_chags);
	queue_delayed_work(queue,&work,delay);
	printk(KERN_INFO "DONE!!! entered into %s function \n",__func__ );
	return 0;
}

void model_finalize(void){

	terminate=1;
	cancel_delayed_work(&work);
	flush_workqueue(queue);
	destroy_workqueue(queue);
	printk(KERN_INFO "entered into %s function \n",__func__ );
}

module_init(model_initialize);
module_exit(model_finalize);