#include <linux/sched.h>
#include "rt_task.h"
#include "defs.h"

void set_task_id(int id)
{
	current->rt_task.id = id;
	
}

void set_task_D(unsigned long long relative_deadline){
	current->rt_task.data.edf.D = relative_deadline;
	
	#ifdef CONSOLE_DEBUGGING 
   	printk(KERN_DEBUG "set_task_D: %llu\n", current->rt_task.data.edf.D);
	#endif
}
