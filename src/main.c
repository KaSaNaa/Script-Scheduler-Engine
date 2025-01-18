#include "scheduler.h"
#include "func.h"
#include <unistd.h>

int main(void) {
    initialize_scheduler();
    start_scheduler_thread();

    Task task1 = {"scripts/test1.py", 1};
    Task task2 = {"scripts/test2.sh", 2};

    add_task(task1);
    add_task(task2);

    setup_signal_handlers();
    start_watchdog();
    
    while (1) {
        pause();
    }
    
    return 0;
}