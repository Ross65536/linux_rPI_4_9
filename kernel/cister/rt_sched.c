#include <linux/rbtree.h>
#include <linux/types.h>

#include "../sched/sched.h"
#include "defs.h"


void push_node_to_rb_tree(struct rb_root * root, struct task_struct *new_task)
{
	struct rb_node **n = &root->rb_node;
	struct rb_node *parent = *n;
	struct rt_task * ans;
	unsigned long long target = new_task->rt_task.d;

	while(*n)
	{
		parent = *n;
		ans = rb_entry(parent, struct rt_task, node);
		if(target < ans->d)
			n = &parent->rb_left;
		else //if(target >= ans->d)
			n = &parent->rb_right;
	}

	rb_link_node(&new_task->rt_task.node, parent, n);
	rb_insert_color(&new_task->rt_task.node, root);

	return ;
}

static struct task_struct* get_first_task_from_root(struct rb_root* root)
{
	struct rb_node * first_node;
	struct rt_task * first_rt_task;
	struct task_struct* p;

	first_node = rb_first(root);
	if(first_node == NULL)
	{
		#ifdef CONFIG_CISTER_TRACING
			printk(KERN_DEBUG "get_first_task_from_root: most priority is null \n");
		#endif	
		return NULL;
	}

	first_rt_task = rb_entry(first_node, struct rt_task, node);
	
	p= container_of(first_rt_task, struct task_struct, rt_task);

#ifdef CONFIG_CISTER_TRACING
	printk(KERN_DEBUG "get_first_task_from_root: most priority is NOT null \n");
#endif	

	return p;
}

static void enqueue_task_rt(struct rq *rq, struct task_struct *p, int flags)
{
	
	s64 time_now = ktime_to_ns(ktime_get());
	
	p->rt_task.d = time_now + p->rt_task.D; //setting absolute time

	#ifdef CONSOLE_DEBUGGING 
	printk(KERN_DEBUG "enqueue_task_rt: time_now %lld, id %d, d %llu, D %llu \n", time_now, p->rt_task.id,
	p->rt_task.d,p->rt_task.D);
	#endif

	spin_lock(&rq->rt_rqs.edf.lock);

	push_node_to_rb_tree(&rq->rt_rqs.edf.tasks_root, p);
	rq->rt_rqs.edf.highest_priority_task = get_first_task_from_root(&rq->rt_rqs.edf.tasks_root);

	spin_unlock(&rq->rt_rqs.edf.lock);

#ifdef CONFIG_CISTER_TRACING
	cister_trace(ENQUEUE_RQ,p);
	cister_trace(MOST_PRIORITY,rq->rt_rqs.edf.highest_priority_task);
#endif

}

static void dequeue_task_rt(struct rq *rq, struct task_struct *p, int flags)
{
	spin_lock(&rq->rt_rqs.edf.lock);

	rb_erase(&p->rt_task.node, &rq->rt_rqs.edf.tasks_root);
	rq->rt_rqs.edf.highest_priority_task = get_first_task_from_root(&rq->rt_rqs.edf.tasks_root);
	
	spin_unlock(&rq->rt_rqs.edf.lock);

#ifdef CONFIG_CISTER_TRACING
	cister_trace(DEQUEUE_RQ,p);
	cister_trace(MOST_PRIORITY,rq->rt_rqs.edf.highest_priority_task);
#endif

	#ifdef CONSOLE_DEBUGGING 
	printk(KERN_DEBUG "dequeue_task_rt: id %d, d %llu, D %llu \n", p->rt_task.id,
	p->rt_task.d,p->rt_task.D);
	#endif
}

static void check_preempt_curr_rt(struct rq *rq, struct task_struct *p, int flags)
{
	printk(KERN_DEBUG "check_preempt_curr_rt: hit %d \n", p->policy);

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
			
			if(rq->rt_rqs.edf.highest_priority_task != rq->curr)
				resched_curr(rq);
			break;

		default:
			break;
	}
}

static struct task_struct *pick_next_task_rt(struct rq *rq, struct task_struct *prev, struct pin_cookie cookie)
{
	struct task_struct * p = NULL;
	spin_lock(&rq->rt_rqs.edf.lock);
	p = rq->rt_rqs.edf.highest_priority_task;
	spin_unlock(&rq->rt_rqs.edf.lock);
	
	return p;
}


///////////////////////////////////////////////
//// Shouldn't change
///////////////////////////////////////////////


static void task_tick_rt(struct rq *rq, struct task_struct *curr, int queued)
{
}

static void yield_task_rt(struct rq *rq)
{
}

static void put_prev_task_rt(struct rq *rq, struct task_struct *prev)
{
}

#ifdef CONFIG_SMP
static int select_task_rq_rt(struct task_struct *p, int task_cpu2, int sd_flag, int flags)
{
	return task_cpu(p);
}
#endif /* CONFIG_SMP */

static void set_curr_task_rt(struct rq *rq)
{
}



static unsigned int get_rr_interval_rt(struct rq *rq, struct task_struct *task)
{
	return 0;
}
static void switched_to_rt(struct rq *rq, struct task_struct *p)
{
}

static void prio_changed_rt(struct rq *rq, struct task_struct *p, int oldprio)
{
}

static void update_curr_rt(struct rq *rq)
{
}

const struct sched_class rts_sched_class = {
	.next = &fair_sched_class,
	.enqueue_task = enqueue_task_rt,
	.dequeue_task = dequeue_task_rt,
	.yield_task = yield_task_rt,
	.check_preempt_curr = check_preempt_curr_rt,
	.pick_next_task = pick_next_task_rt,
	.put_prev_task = put_prev_task_rt,
	#ifdef CONFIG_SMP
	.select_task_rq = select_task_rq_rt,
	#endif
	.set_curr_task = set_curr_task_rt,
	.task_tick = task_tick_rt,
	.get_rr_interval = get_rr_interval_rt,
	.prio_changed = prio_changed_rt,
	.switched_to = switched_to_rt,
	.update_curr = update_curr_rt,
};

