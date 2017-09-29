#include "edf_rq.h"

void init_edf_rq(struct edf_rq *rq)
{
	printk(KERN_DEBUG "init_edf_rq: called\n");
	rq->tasks = RB_ROOT;
	spin_lock_init(&rq->lock);
	rq->task = NULL;
}