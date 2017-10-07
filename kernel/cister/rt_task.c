#include <linux/sched.h>
#include "rt_task.h"
#include "defs.h"

void set_task_id(int id)
{
	current->rt_task.id = id;
	
}

int set_task_rt_subsched_and_param(struct task_struct* curr_task, enum rt_scheduler scheduler, unsigned long long param)
{
	struct rt_task* task = &curr_task->rt_task;

	if(scheduler < 0 || scheduler >= NUM_RT_SCHEDULLERS)
	{
		printk(KERN_INFO "process pid: %d rt subscheduler index %d is not valid", curr_task->pid, scheduler);
		return -2;
	}

	//TODO add rescheduling support

	task->scheduler = scheduler;
	
	if(scheduler == EDF_INDEX)
		task->data.edf.D = param;
	else
		task->tree_key = param;
	
   	PRINT_DEBUG_MESSAGE_WITH_ARGS("scheduler index: %d, param: %llu\n", scheduler, param);

   	return 0;
}
