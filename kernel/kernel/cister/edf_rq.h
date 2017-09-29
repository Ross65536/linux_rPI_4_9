#ifndef __EDF_RQ_H_
#define __EDF_RQ_H_

#include <linux/sched.h>
#include <linux/list.h>
#include <linux/spinlock.h>

struct edf_rq
{
	struct rb_root tasks;
	struct task_struct *task; // first of rb_tree
	spinlock_t lock;
};

void init_edf_rq(struct edf_rq *rq);

#endif 
