#ifndef __CISTER_SYSCALLS_H
#define __CISTER_SYSCALLS_H

asmlinkage long sys_cister_tracing(int enable);

asmlinkage long sys_cister_set_task_id(int id);

asmlinkage long sys_cister_set_rt_subscheduler_and_params(int rt_subscheduler, long subscheduler_parameter);







#endif
