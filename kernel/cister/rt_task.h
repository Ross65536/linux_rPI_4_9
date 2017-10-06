#ifndef __RT_TASKS_H_W
#define __RT_TASKS_H_W

#include <linux/rbtree.h>

#include "rt_common.h"


struct edf_data
{
	unsigned long long d; //absolute deadline
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
	tree_key_type tree_key;
	union rt_data data;
};


void set_task_id(int id);

void set_task_D(unsigned long long relative_deadline);

#endif
