#include "rt_common.h"
#include "rt_rqs.h"
#include "rt_task.h"
#include "../sched/sched.h"
#include "defs.h"

static inline void calc_update_edf_tree_key(struct rt_task* task_ptr)
{
	const s64 time_now = ktime_to_ns(ktime_get());
	task_ptr->tree_key = time_now + task_ptr->data.edf.D;
}

#define SEC_TO_NSEC 1000000000ULL
static inline enum rt_scheduler get_and_check_subscheduler_index(struct rt_task *task)
{
	enum rt_scheduler scheduler_index = task->scheduler;

	if(scheduler_index < 0 || scheduler_index >= NUM_RT_SCHEDULLERS)
	{	
		printk(KERN_INFO "task with id %d has invalid rt subscheduler index: %d, picking by default EDF with 1 second relative deadline\n", task->id, (int) scheduler_index);
		scheduler_index = EDF_INDEX;
		task->data.edf.D = SEC_TO_NSEC;
	}

	return scheduler_index;
}

void init_tree_rq(struct tree_rt_rq *rq, enum rt_scheduler sched_index)
{
	rq->tasks_root = RB_ROOT;
	spin_lock_init(&rq->lock);
	rq->highest_priority_task = NULL;
	
}

static void push_node_to_rb_tree(struct rb_root * root, struct rt_task *new_task)
{
	struct rb_node **n = &root->rb_node;
	struct rb_node *parent = *n;
	struct rt_task * comparison_node;
	const tree_key_type new_node_key = new_task->tree_key;

	while(*n)
	{
		parent = *n;
		comparison_node = rb_entry(parent, struct rt_task, node);
		if(new_node_key < comparison_node->tree_key)
			n = &parent->rb_left;
		else 
			n = &parent->rb_right;
	}

	rb_link_node(&new_task->node, parent, n);
	rb_insert_color(&new_task->node, root);

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
		return NULL;
	}

	first_rt_task = rb_entry(first_node, struct rt_task, node);
	
	p = container_of(first_rt_task, struct task_struct, rt_task);

	return p;
}

static inline struct tree_rt_rq * get_tree_rq_from_task(struct rt_rqs * curr_rq, struct rt_task *task)
{
	const enum rt_scheduler scheduler_index = task->scheduler;
	return &curr_rq->rqs[scheduler_index];
}

void enqueue_task_tree_rt(struct rt_rqs *curr_rq, struct rt_task *task_to_enqueue)
{
	struct tree_rt_rq * task_tree_rq;
	enum rt_scheduler scheduler_index;

	scheduler_index = get_and_check_subscheduler_index(task_to_enqueue);
	task_tree_rq = &curr_rq->rqs[scheduler_index];
	
	spin_lock(&task_tree_rq->lock);

	//TODO: maybe make more generic
	if(scheduler_index == EDF_INDEX)
		calc_update_edf_tree_key(task_to_enqueue);

	push_node_to_rb_tree(&task_tree_rq->tasks_root, task_to_enqueue);
	task_tree_rq->highest_priority_task = get_first_task_from_root(&task_tree_rq->tasks_root);

	spin_unlock(&task_tree_rq->lock);

#ifdef CONFIG_CISTER_TRACING
	cister_trace(ENQUEUE_RQ, container_of(task_to_enqueue, struct task_struct, rt_task));
	cister_trace(MOST_PRIORITY,task_tree_rq->highest_priority_task);
#endif

}

void dequeue_task_tree_rt(struct rt_rqs * curr_rq, struct rt_task *task_to_dequeue)
{
	struct tree_rt_rq * task_tree_rq = get_tree_rq_from_task(curr_rq, task_to_dequeue);

	spin_lock(&task_tree_rq->lock);

	rb_erase(&task_to_dequeue->node, &task_tree_rq->tasks_root);
	task_tree_rq->highest_priority_task = get_first_task_from_root(&task_tree_rq->tasks_root);
	
	spin_unlock(&task_tree_rq->lock);

#ifdef CONFIG_CISTER_TRACING
	cister_trace(DEQUEUE_RQ,container_of(task_to_dequeue, struct task_struct, rt_task));
	cister_trace(MOST_PRIORITY,task_tree_rq->highest_priority_task);
#endif

}