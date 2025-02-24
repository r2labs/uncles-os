/* -*- mode: c; c-basic-offset: 4; -*- */
/* Created by Hershal Bhave and Eric Crosson 2015-03-07 */
/* Revision history: Look in Git FGT */
#ifndef __SCHEDULE__
#define __SCHEDULE__

/*! \addtogroup
 * @{
 */
#ifdef __cplusplus
extern "C"
{
#endif

/* We are using the priority scheduler */

/* Recognized priority schedulers */
#include "priority_schedule_structures.h"

#define SYSTICKS_PER_HZ                  80000000
#define MAX_SYSTICKS_PER_HZ              16777216
#define SYSCTLCLOCK                      16000000

/*! Statically allocated multiple queues of tasks */
static sched_task_pool SCHEDULER_TASK_QUEUES[SCHEDULER_MAX_THREADS];

/*! Doubly linked list of unused task queues */
static sched_task_pool* SCHEDULER_UNUSED_QUEUES = NULL;

/*! UTHash of live task queues */
/* for now it's a utlist though, don't get confused */
static sched_task_pool* SCHEDULER_QUEUES = NULL;

/*! Statically allocated task metadata structures for the scheduler to
 *  manage */
static sched_task SCHEDULER_TASKS[SCHEDULER_MAX_THREADS];

/*! Doubly linked list of unused tasks */
static sched_task* SCHEDULER_UNUSED_TASKS = NULL;


/*! Initialize all deep datastructures used by libschedule. */
void schedule_init();

void schedule(task_t, frequency_t);

sched_task_pool* schedule_hash_find_int(sched_task_pool* queues,
                                        frequency_t target_frequency);

void schedule_hash_add_int(sched_task_pool* queues, sched_task_pool* add);

sched_task* edf_get_edf_queue();
void _os_choose_next_thread();

#ifdef __cplusplus
}
#endif

#endif  /* __SCHEDULE__ */

/* End Doxygen group
 * @}
 */
