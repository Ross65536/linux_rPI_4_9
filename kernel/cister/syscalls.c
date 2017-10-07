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


asmlinkage long sys_cister_set_rt_subscheduler_and_params(int rt_subscheduler, unsigned long subscheduler_parameter_lower_4_bytes, unsigned long subscheduler_parameter_higher_4_bytes)
{
	unsigned long long param;

#ifdef CONFIG_CISTER_RT_SCHEDULERS
	PRINT_DEBUG_MESSAGE_WITH_ARGS("sched: %d, param low 4B: 0x%lx, param high 4B: 0x%lx\n", rt_subscheduler, subscheduler_parameter_lower_4_bytes, subscheduler_parameter_higher_4_bytes);

	param = ( (unsigned long long) subscheduler_parameter_higher_4_bytes) << 32ULL;
	param += (unsigned long long) subscheduler_parameter_lower_4_bytes;

	return set_task_rt_subsched_and_param(current, rt_subscheduler, param);
#else
	return -1;
#endif
}
