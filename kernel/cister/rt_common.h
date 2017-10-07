#ifndef __RT_COMMONH_H__W
#define __RT_COMMONH_H__W


/**
 * Must be an index, lower index indicates a higher priority of the scheduler
 */
enum rt_scheduler
{
	EDF_INDEX = 0,
	DM_INDEX = 1,
	RM_INDEX = 2
};

#define NUM_RT_SCHEDULLERS 3

typedef unsigned long long tree_key_type;


















#endif
