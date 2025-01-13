#include "scheduler.h"
#include "func.h"
#include <stdio.h>
#include <stdbool.h>

#define MAX_TASKS 10

static Task task_queue[MAX_TASKS];
static int task_count = 0;

void initialize_scheduler(void) {
    // ! Initialize the scheduler (setup necessary data structures, etc.)
    printf("Scheduler Initialized.\n");
}

void run_scheduler(void) {
    printf("Scheduler Running.\n");

    for (int i = 0; i < task_count; i++)
    {
        printf("Executing script: %s\n", task_queue[i].script_name);
        execute_script(task_queue[i].script_name);
    }
    
}

bool add_task(Task task) {
    if (task_count >= MAX_TASKS) {
        return false;
    }
    task_queue[task_count++] = task;
    return true;
}