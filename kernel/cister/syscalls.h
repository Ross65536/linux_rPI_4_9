#ifndef __CISTER_SYSCALLS_H
#define __CISTER_SYSCALLS_H

asmlinkage long sys_cister_tracing(int enable);

asmlinkage long sys_cister_set_rt_subscheduler_and_params(int rt_subscheduler, unsigned long subscheduler_parameter_lower_4_bytes, unsigned long subscheduler_parameter_higher_4_bytes);

asmlinkage long sys_cister_set_task_id(int id);




#endif
