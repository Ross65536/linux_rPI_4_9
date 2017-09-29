#include <linux/sched.h>
#include "edf_task.h"
#include "defs.h"

void set_task_id(int id)
{
	current->edf_task.id = id;
	
}

void set_task_D(unsigned long long relative_deadline){
	current->edf_task.D = relative_deadline;
	
	#ifdef CONSOLE_DEBUGGING 
   	printk(KERN_DEBUG "set_task_D: %llu\n", current->edf_task.D);
	#endif
}
