#include <linux/rbtree.h>
#include <linux/types.h>


#include "defs.h"
#include "rt_rqs.h"
#include "rt_rqs_functions.h"
#include "../sched/sched.h"

static void enqueue_task_rts(struct rq *rq, struct task_struct *new_task, int flags)
{
	enqueue_task_tree_rt(&rq->rt_rqs, &new_task->rt_task);
}

static void dequeue_task_rts(struct rq *rq, struct task_struct *old_task, int flags)
{
	dequeue_task_tree_rt(&rq->rt_rqs, &old_task->rt_task);
}

static void check_preempt_curr_rts(struct rq *rq, struct task_struct *p, int flags)
{

	switch(rq->curr->policy){
		case SCHED_DEADLINE:
		case SCHED_FIFO:
		case SCHED_RR:
			break;

		case SCHED_NORMAL:
		case SCHED_BATCH:
		case SCHED_IDLE:
			resched_curr(rq);	
			break;
		case SCHED_RTS:
			if(check_preempt_rts_tasks(&rq->rt_rqs, &rq->curr->rt_task, &p->rt_task))
				resched_curr(rq);
			break;
		default:
			break;
	}
}

static struct task_struct *pick_next_task_rts(struct rq *rq, struct task_struct *prev, struct pin_cookie cookie)
{
	return pick_next_task_from_rts(&rq->rt_rqs);
}


///////////////////////////////////////////////
//// Shouldn't change
///////////////////////////////////////////////


static void task_tick_rts(struct rq *rq, struct task_struct *curr, int queued)
{
}

static void yield_task_rts(struct rq *rq)
{
}

static void put_prev_task_rts(struct rq *rq, struct task_struct *prev)
{
}

#ifdef CONFIG_SMP
static int select_task_rq_rts(struct task_struct *p, int task_cpu2, int sd_flag, int flags)
{
	return task_cpu(p);
}
#endif /* CONFIG_SMP */

static void set_curr_task_rts(struct rq *rq)
{
}



static unsigned int get_rr_interval_rts(struct rq *rq, struct task_struct *task)
{
	return 0;
}
static void switched_to_rts(struct rq *rq, struct task_struct *p)
{
}

static void prio_changed_rts(struct rq *rq, struct task_struct *p, int oldprio)
{
}

static void update_curr_rts(struct rq *rq)
{
}

const struct sched_class rts_sched_class = {
	.next = &fair_sched_class,
	.enqueue_task = enqueue_task_rts,
	.dequeue_task = dequeue_task_rts,
	.yield_task = yield_task_rts,
	.check_preempt_curr = check_preempt_curr_rts,
	.pick_next_task = pick_next_task_rts,
	.put_prev_task = put_prev_task_rts,
	#ifdef CONFIG_SMP
	.select_task_rq = select_task_rq_rts,
	#endif
	.set_curr_task = set_curr_task_rts,
	.task_tick = task_tick_rts,
	.get_rr_interval = get_rr_interval_rts,
	.prio_changed = prio_changed_rts,
	.switched_to = switched_to_rts,
	.update_curr = update_curr_rts,
};

