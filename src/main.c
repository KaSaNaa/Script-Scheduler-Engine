#include "scheduler.h"

int main(void) {
    initialize_scheduler();
    
    Task task1 = {1, "scripts/test1.py"};
    Task task2 = {2, "scripts/test2.sh"};

    add_task(task1);
    add_task(task2);

    run_scheduler();
    return 0;
}