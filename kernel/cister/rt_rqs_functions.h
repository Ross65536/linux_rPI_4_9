#ifndef _RT_RQS_FUNCTION_H__
#define _RT_RQS_FUNCTION_H__

#include "rt_rqs.h"
#include "rt_task.h"
#include "../sched/sched.h"

void init_tree_rq(struct tree_rt_rq *rq, enum rt_scheduler sched_index);

struct task_struct * pick_next_task_from_rts(struct rt_rqs* rqs);

void enqueue_task_tree_rt(struct rt_rqs * curr_rq, struct rt_task *task_to_enqueue);

void dequeue_task_tree_rt(struct rt_rqs * curr_rq, struct rt_task *task_to_dequeue);

bool check_preempt_rts_tasks(struct rt_rqs* rqs, struct rt_task * curr_task, struct rt_task * new_task);









#endif
