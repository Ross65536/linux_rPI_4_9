#ifndef __DEFS_H__
#define __DEFS_H__



#define CONSOLE_DEBUGGING


#ifdef CONSOLE_DEBUGGING
#include <linux/kernel.h>
#endif

/* printk template
#ifdef CONSOLE_DEBUGGING 
printk(KERN_DEBUG "msg");
#endif
*/



#endif