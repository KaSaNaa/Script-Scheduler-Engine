#include "scheduler.h"
#include "func.h"
#include <stdio.h>

void initialize_scheduler(void) {
    // ! Initialize the scheduler (setup necessary data structures, etc.)
    printf("Scheduler Initialized.\n");
}

void run_scheduler(void) {
    printf("Scheduler Running.\n");

    printf("Executing script: test1.py\n");
    execute_script("scripts/test1.py");

    printf("Executing script: test2.sh\n");
    execute_script("scripts/test2.sh");
}
