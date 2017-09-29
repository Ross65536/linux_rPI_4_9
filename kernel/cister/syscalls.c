#include <linux/syscalls.h>
#include "trace.h"
#include "defs.h"


asmlinkage long sys_cister_tracing(int enable)
{
#ifdef CONFIG_CISTER_TRACING
	enable_tracing(enable);
	return 0;
#else
	return -1;
#endif
}

asmlinkage long sys_cister_set_task_id(int id)
{
#ifdef CONFIG_CISTER_RT_SCHEDULERS
	set_task_id(id);
	return 0;
#else
	return -1;
#endif	
}


asmlinkage long sys_cister_set_task_rd(unsigned int relative_deadline)
{

#ifdef CONSOLE_DEBUGGING 
   printk(KERN_DEBUG "sys_cister_set_task_rd: %u\n", relative_deadline);
#endif 

#ifdef CONFIG_CISTER_RT_SCHEDULERS
	unsigned long long val = (unsigned long long ) relative_deadline;
	set_task_D(val);
	return 0;
#else
	return -1;
#endif
}
