#include <linux/types.h>

#include "rt_rqs.h"



void init_rt_rqs(struct rt_rqs *rqs)
{
	int i;

	printk(KERN_DEBUG "init_rt_rqs: called\n");

	for(i=0; i < NUM_RT_SCHEDULLERS; i++)
		init_tree_rq(&rqs->rqs[i], i);

}

struct task_struct * pick_next_task_from_rts(struct rt_rqs* rqs)
{
	struct task_struct * high_pri;
	int i=0;

	for(; i < NUM_RT_SCHEDULLERS; i++)
	{
		spin_lock(&rqs->rqs[i].lock);
		high_pri = rqs->rqs[i].highest_priority_task;
		spin_unlock(&rqs->rqs[i].lock);

		if(high_pri != NULL)
			return high_pri;
	}

	return NULL;
}

bool check_preempt_rts_tasks(struct rq* rqs, struct rt_task * curr_task, struct rt_task * new_task)
{
	if(new_task->scheduler < curr_task->scheduler)
		return true;
	else if (new_task->scheduler == curr_task->scheduler)
	{
		const enum rt_scheduler scheduler_index = new_task->scheduler;
		const struct task_struct * high_pri = rqs->rt_rqs.rqs[scheduler_index].highest_priority_task;
		if ( &high_pri->rt_task != curr_task)
			return true;
	}
	
	return false;
}