#include <stdbool.h>

#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct {
    int priority;
    char *script_name;
    // A simple way to prioritize tasks.
    // * Other fields like status, execution time, etc. can be added here.
} Task;

void initialize_scheduler(void);
void run_scheduler(void);
bool add_task(Task task);

#endif // SCHEDULER_H
