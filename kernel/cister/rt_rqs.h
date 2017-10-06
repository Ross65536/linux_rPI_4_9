#ifndef __EDF_RQ_H_
#define __EDF_RQ_H_

#include <linux/sched.h>
#include <linux/list.h>
#include <linux/spinlock.h>

struct edf_rq
{
	struct rb_root tasks_root;
	struct task_struct *highest_priority_task; // first of rb_tree
	spinlock_t lock;
};

struct rt_rqs
{
    struct edf_rq edf;
};


void init_rt_rqs(struct rt_rqs *rq);


#endif 
