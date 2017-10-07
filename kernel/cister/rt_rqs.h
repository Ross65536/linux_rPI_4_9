#ifndef __RT_RQ_H_
#define __RT_RQ_H_

#include <linux/sched.h>
#include <linux/rbtree.h>
#include <linux/spinlock.h>

#include "rt_common.h"

//typedef tree_key_type (* tree_key_calc_function) (struct rt_task *);

struct tree_rt_rq
{
	struct rb_root tasks_root;
	struct task_struct *highest_priority_task; // first of rb_tree
	spinlock_t lock;
};

struct rt_rqs
{
    struct tree_rt_rq rqs[NUM_RT_SCHEDULLERS];
};

void init_rt_rqs(struct rt_rqs *rq);





#endif 
