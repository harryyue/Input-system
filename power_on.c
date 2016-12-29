#include <linux/module.h>
#include <linux/input.h>
#include <linux/bitops.h>
#include <linux/export.h>
#include "bt_wakeup.h"

static struct input_dev *button_dev;  

void bcm_wakeup(void)  
{  
	input_report_key(button_dev, KEY_WAKEUP, 1);  
	input_sync(button_dev);  
}

static int __init button_init(void)  
{  
	int error;   

	//	if (request_irq(BUTTON_IRQ, button_interrupt, 0, "button",NULL)) {  
	//		printk(KERN_ERR"button.c: Can't allocate irq %d\n", button_irq);  
	//		return -EBUSY;  
	//	}        

	button_dev = input_allocate_device();  
	if (!button_dev) {  
		printk(KERN_ERR"button.c: Not enough memory\n");  
		error = -ENOMEM;  
		goto err_free_irq;  
	}  

	//button_dev->evbit[0] = BIT(EV_KEY);  
	//button_dev->keybit[LONG(KEY_POWER)] = BIT(KEY_POWER);  

	button_dev->evbit[0] = BIT_MASK(EV_KEY);                                                                                                                                
	button_dev->keybit[BIT_WORD(KEY_WAKEUP)] = BIT_MASK(KEY_WAKEUP);

	error = input_register_device(button_dev);  
	if (error) {  
		printk(KERN_ERR"button.c: Failed to register device\n");  
		goto err_free_dev;  
	}  

	//input_report_key(button_dev, KEY_WAKEUP , 1);  
	//input_sync(button_dev);   

	return 0;  

err_free_dev:  
	input_free_device(button_dev);  
err_free_irq:  
	//free_irq(BUTTON_IRQ, button_interrupt);  
	return error;  
}  

static void __exit button_exit(void)  
{  
	input_unregister_device(button_dev);  
	//free_irq(BUTTON_IRQ, button_interrupt);  
}  

module_init(button_init);  
module_exit(button_exit);   

MODULE_LICENSE("GPL");
