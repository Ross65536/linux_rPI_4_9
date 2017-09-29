#include "../sched/sched.h"
#include <linux/rbtree.h>
#include "defs.h"

/*
* EDF scheduling class.
* Implements SCHED_EDF
*/

void push_node_to_rb_tree(struct rb_root * root, struct task_struct *new_task)
{
	struct rb_node **n = &root->rb_node;
	struct rb_node *parent = *n;
	struct edf_task * ans;
	unsigned long long target = new_task->edf_task.d;

	while(*n)
	{
		parent = *n;
		ans = rb_entry(parent, struct edf_task, node);
		if(target < ans->d)
			n = &parent->rb_left;
		else //if(target >= ans->d)
			n = &parent->rb_right;
	}

	rb_link_node(&new_task->edf_task.node, parent, n);
	rb_insert_color(&new_task->edf_task.node, root);

	return ;
}

static struct task_struct* get_first_task_from_root(struct rb_root* root)
{
	struct rb_node * first_node;
	struct edf_task * first_edf_task;
	struct task_struct* p;

	first_node = rb_first(root);
	if(first_node == NULL)
	{
		#ifdef CONFIG_CISTER_TRACING
			printk(KERN_DEBUG "get_first_task_from_root: most priority is null \n");
		#endif	
		return NULL;
	}

	first_edf_task = rb_entry(first_node, struct edf_task, node);
	
	p= container_of(first_edf_task, struct task_struct, edf_task);

#ifdef CONFIG_CISTER_TRACING
		cister_trace(MOST_PRIORITY,p);
#endif	
#ifdef CONFIG_CISTER_TRACING
	printk(KERN_DEBUG "get_first_task_from_root: most priority is NOT null \n");
#endif	

	return p;
}

static void enqueue_task_edf(struct rq *rq, struct task_struct *p, int flags)
{
	
	ktime_t time_now = ktime_to_ns(ktime_get());
	
	p->edf_task.d = time_now + p->edf_task.D; //setting absolute time

	#ifdef CONSOLE_DEBUGGING 
	printk(KERN_DEBUG "enqueue_task_edf: time_now %lld, id %d, d %llu, D %llu \n", time_now, p->edf_task.id,
	p->edf_task.d,p->edf_task.D);
	#endif

	spin_lock(&rq->edf.lock);

	push_node_to_rb_tree(&rq->edf.tasks, p);
	rq->edf.task = get_first_task_from_root(&rq->edf.tasks);

	spin_unlock(&rq->edf.lock);

#ifdef CONFIG_CISTER_TRACING
	cister_trace(ENQUEUE_RQ,p);
#endif

}

static void dequeue_task_edf(struct rq *rq, struct task_struct *p, int flags)
{
	spin_lock(&rq->edf.lock);

	rb_erase(&p->edf_task.node, &rq->edf.tasks);
	rq->edf.task = get_first_task_from_root(&rq->edf.tasks);
	
	spin_unlock(&rq->edf.lock);

#ifdef CONFIG_CISTER_TRACING
	cister_trace(DEQUEUE_RQ,p);
#endif

	#ifdef CONSOLE_DEBUGGING 
	printk(KERN_DEBUG "dequeue_task_edf: id %d, d %llu, D %llu \n", p->edf_task.id,
	p->edf_task.d,p->edf_task.D);
	#endif
}

static void check_preempt_curr_edf(struct rq *rq, struct task_struct *p, int flags)
{
	printk(KERN_DEBUG "check_preempt_curr_edf: hit %d \n", p->policy);

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
		case SCHED_EDF:
			
			if(rq->edf.task != rq->curr)
				resched_curr(rq);
			break;

		default:
			break;
	}
}

static struct task_struct *pick_next_task_edf(struct rq *rq, struct task_struct *prev, struct rq_flags *rf)
{
	struct task_struct * p = NULL;
	spin_lock(&rq->edf.lock);
	p = rq->edf.task;
	spin_unlock(&rq->edf.lock);
	if(p == NULL)
		printk(KERN_DEBUG "pick_next_task_edf: p is null \n");
	return p;
}

static void task_tick_edf(struct rq *rq, struct task_struct *curr, int queued)
{
}






///////////////////////////////////////////////
//// Shouldn't change
///////////////////////////////////////////////

static void yield_task_edf(struct rq *rq)
{
}

static void put_prev_task_edf(struct rq *rq, struct task_struct *prev)
{
}

#ifdef CONFIG_SMP
static int select_task_rq_edf(struct task_struct *p, int task_cpu2, int sd_flag, int flags)
{
	return task_cpu(p);
}
#endif /* CONFIG_SMP */

static void set_curr_task_edf(struct rq *rq)
{
}



static unsigned int get_rr_interval_edf(struct rq *rq, struct task_struct *task)
{
	return 0;
}
static void switched_to_edf(struct rq *rq, struct task_struct *p)
{
}

static void prio_changed_edf(struct rq *rq, struct task_struct *p, int oldprio)
{
}

static void update_curr_edf(struct rq *rq)
{
}

const struct sched_class edf_sched_class = {
	.next = &fair_sched_class,
	.enqueue_task = enqueue_task_edf,
	.dequeue_task = dequeue_task_edf,
	.yield_task = yield_task_edf,
	.check_preempt_curr = check_preempt_curr_edf,
	.pick_next_task = pick_next_task_edf,
	.put_prev_task = put_prev_task_edf,
	#ifdef CONFIG_SMP
	.select_task_rq = select_task_rq_edf,
	#endif
	.set_curr_task = set_curr_task_edf,
	.task_tick = task_tick_edf,
	.get_rr_interval = get_rr_interval_edf,
	.prio_changed = prio_changed_edf,
	.switched_to = switched_to_edf,
	.update_curr = update_curr_edf,
};

