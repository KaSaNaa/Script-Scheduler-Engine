#include "scheduler.h"

int main(void) {
    initialize_scheduler();
    
    Task task1 = {"scripts/test1.py", 1};
    Task task2 = {"scripts/test2.sh", 2};

    add_task(task1);
    add_task(task2);

    run_scheduler();
    return 0;
}