#include "rt_rqs.h"

void init_edf_rq(struct edf_rq *rq)
{
	rq->tasks_root = RB_ROOT;
	spin_lock_init(&rq->lock);
	rq->highest_priority_task = NULL;
}

void init_rt_rqs(struct rt_rqs *rqs)
{
	printk(KERN_DEBUG "init_rt_rq: called\n");
	init_edf_rq(&rqs->edf);
}