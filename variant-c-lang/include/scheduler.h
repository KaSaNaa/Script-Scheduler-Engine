#include "priority_queue.h"
#include <pthread.h>
#include <stdbool.h>

#ifndef SCHEDULER_H
#define SCHEDULER_H
#define MAX_TASKS 100

extern PriorityQueue *task_queue;
extern pthread_mutex_t queue_mutex;
extern pthread_cond_t queue_cond;

void initialize_scheduler(void);
void run_scheduler(void);
bool add_task(Task task);
void start_scheduler_thread(void);
void start_config_watcher(void);

#endif // SCHEDULER_H