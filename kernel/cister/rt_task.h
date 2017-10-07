#ifndef __RT_TASKS_H_W
#define __RT_TASKS_H_W

#include <linux/rbtree.h>

#include "rt_common.h"


struct edf_data
{
	unsigned long long D; //relative deadline
};

union rt_data
{
	struct edf_data edf;
};

struct rt_task
{
	struct rb_node node;
	int id;
	enum rt_scheduler scheduler; //the task's rt subscheduler

	/**
	*tree_key is the key of any rb_tree of any subscheduler, trees have smallest key on their left-side
	*This is also the absolute deadline of a EDF subscheduler, the relative deadline of DM, and the period of a RM
	*/
	tree_key_type tree_key; 
	union rt_data data;
};


void set_task_id(int id);

int set_task_rt_subsched_and_param(struct task_struct* curr_task, enum rt_scheduler scheduler, unsigned long long param);







#endif
