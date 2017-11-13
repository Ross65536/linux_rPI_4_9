#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#include "trace.h"
#include "rt_rqs.h"
#include "rt_rqs_functions.h"

///////////////////////////////////////////////////////////////////////
// Ring Buffer implementation
//////////////////////////////////////////////////////////////////////

struct trace_evt_buffer trace;

unsigned char enabled = 0;

static void increment(int *item)
{
	*item = *item + 1;
	if (*item >= TRACE_BUFFER_SIZE)
	{
		*item = 0;
	}
}

static int is_empty(int r, int w)
{
	return !(r ^ w); //xor
}

static int is_full(int r, int w)
{
	int write = w;
	increment(&write);
	return write == r;
}

static void sprint_dequeue_rt_task(int * len, char* buffer, struct trace_evt* evt)
{
	*len += sprintf(buffer + *len, "id,%d,", evt->task_id);
	*len += sprintf(buffer + *len, "bPreempt,%d,", evt->bPreemptible);
	switch (evt->scheduler) 
	{
		case EDF_INDEX:
			*len += sprintf(buffer + *len, "rt_sched,%s,", "EDF");
			*len += sprintf(buffer + *len, "d,%llu,", evt->tree_key);
			*len += sprintf(buffer + *len, "D,%llu,", evt->rt_data.edf.D);
			break;
		case DM_INDEX:
			*len += sprintf(buffer + *len, "rt_sched,%s,", "DM");
			*len += sprintf(buffer + *len, "D,%llu,", evt->tree_key);
			break;
		case RM_INDEX:
			*len += sprintf(buffer + *len, "rt_sched,%s,", "RM");
			*len += sprintf(buffer + *len, "T,%llu,", evt->tree_key);
			break;
		default:
			break;
	}	
}

static int dequeue(char *buffer)
{
	int ret = 0, len;
	char evt[20];
	spin_lock(&trace.lock);
	if (!is_empty(trace.read_item, trace.write_item))
	{ //if it is not empty
		switch ((int)trace.events[trace.read_item].event)
		{
		case SCHED_TICK:
			strcpy(evt, "SCH_TK");
			break;
		case SWITCH_AWAY:
			strcpy(evt, "SWT_AY");
			break;
		case SWITCH_TO:
			strcpy(evt, "SWT_TO");
			break;
		case ENQUEUE_RQ:
			strcpy(evt, "ENQ_RQ");
			break;
		case DEQUEUE_RQ:
			strcpy(evt, "DEQ_RQ");
			break;
		case MOST_PRIORITY:
			strcpy(evt, "MS_PRI");
			break;
		}
		len = sprintf(buffer, "%llu,", trace.events[trace.read_item].time);
		len += sprintf(buffer + len, "%s,", evt);

#ifdef CONFIG_CISTER_RT_SCHEDULERS
		sprint_dequeue_rt_task(&len, buffer, & trace.events[trace.read_item]);
#endif
		len += sprintf(buffer + len, "pid,%d,", (int)trace.events[trace.read_item].pid);
		len += sprintf(buffer + len, "prio,%d,", (int)trace.events[trace.read_item].prio);
		len += sprintf(buffer + len, "policy,%d,", (int)trace.events[trace.read_item].policy);
		len += sprintf(buffer + len, "state,%d,", (int)trace.events[trace.read_item].state);
		len += sprintf(buffer + len, "%s\n", trace.events[trace.read_item].comm);
		increment(&trace.read_item);
		ret = 1;
	}

	spin_unlock(&trace.lock);
	return ret;
}

static int enqueue(enum evt event, unsigned long long time, struct task_struct *p)
{
	spin_lock(&trace.lock);
	if (is_full(trace.read_item, trace.write_item))
		increment(&trace.read_item);
	trace.events[trace.write_item].event = event;
	trace.events[trace.write_item].time = time;
	trace.events[trace.write_item].pid = p->pid;
	trace.events[trace.write_item].state = p->state;
	trace.events[trace.write_item].prio = p->prio;
	trace.events[trace.write_item].policy = p->policy;
	strcpy(trace.events[trace.write_item].comm, p->comm);

#ifdef CONFIG_CISTER_RT_SCHEDULERS
	trace.events[trace.write_item].task_id = p->rt_task.id;
	trace.events[trace.write_item].scheduler = p->rt_task.scheduler;
	trace.events[trace.write_item].tree_key = p->rt_task.tree_key;
	trace.events[trace.write_item].rt_data = p->rt_task.data;
	trace.events[trace.write_item].bPreemptible = p->rt_task.isPreemptible ? 1 : 0;
#endif

	increment(&trace.write_item);
	spin_unlock(&trace.lock);
	return 1;
}

ssize_t trace_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	char buffer[TRACE_BUFFER_LEN];
	int ret = 0, len = 0;
	//printk(KERN_INFO "%s:[%d] read\n", TRACE_ENTRY_NAME, current->pid);
	if (!dequeue(buffer))
		return 0;
	len = strlen(buffer);
	if (len <= 0)
		return -EFAULT;
	if (count < len)
		return -EFAULT;
	ret = copy_to_user(buf, buffer, len);
	if (ret != 0)
		return -EFAULT;
	return len;
}

static const struct file_operations trace_fops = {
	.owner = THIS_MODULE,
	.read = trace_read,
};

static int __init proc_trace_init(void)
{
	proc_create(TRACE_ENTRY_NAME, 0444, NULL, &trace_fops);
	printk("CISTER:/proc/%s created\n", TRACE_ENTRY_NAME);
	spin_lock_init(&trace.lock);
	trace.write_item = 0;
	trace.read_item = 0;
	//	enabled = 1; //enabled by syscall
	return 0;
}

module_init(proc_trace_init);

//This function will be used to get the event.
void cister_trace(enum evt event, struct task_struct *p)
{
	if(p == NULL)
		return;

#if defined(CONFIG_CISTER_RT_SCHEDULERS)
	if (p->policy != SCHED_RTS)
		return;

	if (enabled)
	{	
		unsigned long long time = ktime_to_ns(ktime_get());
		enqueue(event, time, p);
	}
#else
	return;
#endif
}

void enable_tracing(int enable)
{
	enabled = (unsigned char)enable;
}
