#include <linux/types.h>

#include "rt_rqs.h"
#include "rt_rqs_functions.h"
#include "../sched/sched.h"
#include "defs.h"

void init_rt_rqs(struct rt_rqs *rqs)
{
	int i;

	for(i=0; i < NUM_RT_SCHEDULLERS; i++)
		init_tree_rq(&rqs->rqs[i], i);

}

struct task_struct * pick_next_task_from_rts(struct rt_rqs* rqs)
{
	struct task_struct * high_pri;
	struct tree_rt_rq* tree_rq;
	int i;

	for(i = 0; i < NUM_RT_SCHEDULLERS; i++)
	{
		tree_rq = &rqs->rqs[i];
		spin_lock(&tree_rq->lock);
		high_pri = tree_rq->highest_priority_task;
		spin_unlock(&tree_rq->lock);

		if(high_pri != NULL)
			return high_pri;
	}

	return NULL;
}

bool check_preempt_rts_tasks(struct rt_rqs* rqs, struct rt_task * curr_task, struct rt_task * new_task)
{
	if(new_task->scheduler < curr_task->scheduler)
		return true;
	else if (new_task->scheduler == curr_task->scheduler)
	{
		const enum rt_scheduler scheduler_index = new_task->scheduler;
		const struct task_struct * high_pri = rqs->rqs[scheduler_index].highest_priority_task;
		if ( &high_pri->rt_task != curr_task)
			return true;
	}

	return false;
}