#include "rt_common.h"
#include "rt_rqs.h"
#include "rt_task.h"


////////////////////////////////////////////////////
/////// Functions specific to each rt scheduler
////////////////////////////////////////////////////

tree_key_type calc_edf_tree_key(struct rt_task* task_ptr)
{
	const s64 time_now = ktime_to_ns(ktime_get());
	task_ptr->data.edf.d = time_now + task_ptr->data.edf.D;
	return task_ptr->data.edf.d; 
}


static tree_key_type (* calc_tree_key_arr[])(struct rt_task*) 
= {calc_edf_tree_key};

////////////////////////////////////////////////////
/////// Common Core
////////////////////////////////////////////////////

void init_tree_rq(struct tree_rt_rq *rq, enum rt_scheduler sched_index)
{
	rq->tasks_root = RB_ROOT;
	spin_lock_init(&rq->lock);
	rq->highest_priority_task = NULL;
	rq->calc_tree_key = calc_tree_key_arr[sched_index];
}

void push_node_to_rb_tree(struct rb_root * root, struct rt_task *new_task)
{
	struct rb_node **n = &root->rb_node;
	struct rb_node *parent = *n;
	struct rt_task * comparison_node;
	tree_key_type new_node_key = new_task->tree_key;

	while(*n)
	{
		parent = *n;
		comparison_node = rb_entry(parent, struct rt_task, node);
		if(new_node_key < comparison_node->tree_key)
			n = &parent->rb_left;
		else //if(new_node_key >= ans->d)
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


void enqueue_task_tree_rt(struct rq *curr_rq, struct task_struct *task_to_enqueue)
{
	const enum rt_scheduler scheduler_index = task_to_enqueue->rt_task.scheduler;
	struct tree_rt_rq * task_tree_rq = & curr_rq->rt_rqs.rqs[scheduler_index];
	
	spin_lock(&task_tree_rq->lock);

	task_to_enqueue->rt_task.tree_key = task_tree_rq->calc_tree_key(&task_to_enqueue->rt_task);

	push_node_to_rb_tree(&task_tree_rq->tasks_root, &task_to_enqueue->rt_task);
	task_tree_rq->highest_priority_task = get_first_task_from_root(&task_tree_rq->tasks_root);

	spin_unlock(&task_tree_rq->lock);

#ifdef CONFIG_CISTER_TRACING
	cister_trace(ENQUEUE_RQ,task_to_enqueue);
	cister_trace(MOST_PRIORITY,task_tree_rq->highest_priority_task);
#endif

	/*
	#ifdef CONSOLE_DEBUGGING 
	printk(KERN_DEBUG "enqueue_task_rt: time_now %lld, id %d, d %llu, D %llu \n", time_now, p->rt_task.id,
	p->rt_task.d,p->rt_task.D);
	#endif
	*/
}

void dequeue_task_tree_rt(struct rq * curr_rq, struct task_struct *task_to_dequeue)
{
	const enum rt_scheduler scheduler_index = task_to_dequeue->rt_task.scheduler;
	struct tree_rt_rq * task_tree_rq = & curr_rq->rt_rqs.rqs[scheduler_index];

	spin_lock(&task_tree_rq->lock);

	rb_erase(&task_to_dequeue->rt_task.node, &task_tree_rq->tasks_root);
	task_tree_rq->highest_priority_task = get_first_task_from_root(&task_tree_rq->tasks_root);
	
	spin_unlock(&task_tree_rq->lock);

#ifdef CONFIG_CISTER_TRACING
	cister_trace(DEQUEUE_RQ,task_to_dequeue);
	cister_trace(MOST_PRIORITY,task_tree_rq->highest_priority_task);
#endif

	/*	
	#ifdef CONSOLE_DEBUGGING 
	printk(KERN_DEBUG "dequeue_task_rt: id %d, d %llu, D %llu \n", task_to_dequeue->rt_task.id,
	p->rt_task.d,p->rt_task.D);
	#endif
	*/
}