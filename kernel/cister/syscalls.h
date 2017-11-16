#ifndef __CISTER_SYSCALLS_H
#define __CISTER_SYSCALLS_H

/**
 * @brief      Enables or disables tracing for processes using our schedulers
 *
 * @param[in]  enable  0 to disable and 1 to enable.
 *
 * @return     0 on success, ptherwside failure
 */
asmlinkage long sys_cister_tracing(int enable);

/**
 * @brief      Sets the (sub)subscheduler to use. This syscall must be called BEFORE syscall sched_setscheduler
 * 			   subscheduler_parameter (in nanoseconds) should be the relative deadline for EDF and DM, and period for RM
 *
 * @param[in]  rt_subscheduler                        options are EDF (0), DM (1), RM(2)
 * @param[in]  subscheduler_parameter_lower_4_bytes   The subscheduler parameter lower 4 bytes
 * @param[in]  subscheduler_parameter_higher_4_bytes  The subscheduler parameter higher 4 bytes
 * @param[in]  bPreemptable                           true if the process can be preempted by any other process running our scheduler
 *
 * @return     { description_of_the_return_value }
 */
asmlinkage long sys_cister_set_rt_subscheduler_and_params(int rt_subscheduler, unsigned long subscheduler_parameter_lower_4_bytes, unsigned long subscheduler_parameter_higher_4_bytes, int bPreemptable);

asmlinkage long sys_cister_set_task_id(int id);




#endif
