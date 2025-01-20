#include <stdbool.h>
#include <pthread.h>

#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct {
    char *script_name;
    int priority;
    // A simple way to prioritize tasks.
    // * Other fields like status, execution time, etc. can be added here.
} Task;

#define MAX_TASKS 10

extern Task task_queue[MAX_TASKS];
extern int task_count;
extern pthread_mutex_t queue_mutex;
extern pthread_cond_t queue_cond;

void initialize_scheduler(void);
void run_scheduler(void);
bool add_task(Task task);
void start_scheduler_thread(void);

#endif // SCHEDULER_H