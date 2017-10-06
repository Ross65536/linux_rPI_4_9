#ifndef __EDF_TASK_H_
#define __EDF_TASK_H_

#include <linux/rbtree.h>

struct rt_task
{
	struct rb_node node;
	int id;

	unsigned long long d; //absolute deadline
	unsigned long long D; //relative deadline

};


void set_task_id(int id);

void set_task_D(unsigned long long relative_deadline);

#endif
