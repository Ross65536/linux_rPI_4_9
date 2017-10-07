#include <linux/syscalls.h>
#include "trace.h"
#include "defs.h"


asmlinkage long sys_cister_tracing(int enable)
{
	PRINT_DEBUG_MESSAGE("enable tracing hit\n")

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
	current->rt_task.id = id;
	return 0;
#else
	return -1;
#endif	
}


asmlinkage long sys_cister_set_rt_subscheduler_and_params(int rt_subscheduler, long subscheduler_parameter)
{
	unsigned long long param;

#ifdef CONFIG_CISTER_RT_SCHEDULERS
	PRINT_DEBUG_MESSAGE_WITH_ARGS("sched: %d, param: %ld\n", rt_subscheduler, subscheduler_parameter);

	param = (unsigned long long ) subscheduler_parameter; //TODO add long long support

	return set_task_rt_subsched_and_param(current, rt_subscheduler, param);
#else
	return -1;
#endif
}
