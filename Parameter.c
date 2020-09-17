#include <linux/module.h>
#include <linux/kernel.h>

MODULE_AUTHOR("CHAGS");
MODULE_DESCRIPTION("This is a test module that we are trying out");

int number;
module_param(number,int,0774); 
//MODULE_PARAM_DESC(number, "An integer number");

char *words;
module_param(words,charp,0774);
//MODULE_PARAM_DESC(words, "An character value");

int iinit_module(void)
{
	printk(KERN_INFO "Module was loaded\n");
	if(words == NULL)
	{
		printk(KERN_INFO "words value was not provided it is needed for initialization, Please reload the module with correct parameters\n");
		return -5;
	}
	else
	{
		printk(KERN_INFO "Values are %d for number and %s for string\n",number,words);	
	}
	
	return 0;
}

void eexit_module(void)
{
	printk(KERN_INFO "Module was removed\n");
	printk(KERN_INFO "Vaule wad %d\n",number);
}

module_init(iinit_module);
module_exit(eexit_module);