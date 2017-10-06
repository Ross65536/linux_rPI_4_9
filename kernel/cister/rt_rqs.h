#ifndef __RT_RQ_H_
#define __RT_RQ_H_

#include <linux/sched.h>
#include <linux/rbtree.h>
#include <linux/spinlock.h>

#include "rt_task.h"
#include "rt_common.h"

typedef tree_key_type (* tree_key_calc_function) (struct rt_task *);

struct tree_rt_rq
{
	struct rb_root tasks_root;
	struct task_struct *highest_priority_task; // first of rb_tree
	spinlock_t lock;
	tree_key_type (* calc_tree_key)(struct rt_task *);
};

struct rt_rqs
{
    struct tree_rt_rq rqs[NUM_RT_SCHEDULLERS];
};

#include "../sched/sched.h"

void init_tree_rq(struct tree_rt_rq *rq, enum rt_scheduler sched_index);

void init_rt_rqs(struct rt_rqs *rq);

struct task_struct * pick_next_task_from_rts(struct rt_rqs* rqs);

void enqueue_task_tree_rt(struct rq * curr_rq, struct task_struct *task_to_enqueue);

void dequeue_task_tree_rt(struct rq * curr_rq, struct task_struct *task_to_dequeue);

bool check_preempt_rts_tasks(struct rq* rqs, struct rt_task * curr_task, struct rt_task * new_task);

#endif 
