#include <scheduler.h>
#include <func.h>
#include <unistd.h>

int main(void) {
    initialize_scheduler();
    start_scheduler_thread();

    char *script1 = "scripts/test1.py";
    char *script2 = "scripts/test2.sh";

    Task task1 = {script1, 1};
    Task task2 = {script2, 2};

    add_task(task1);
    add_task(task2);

    setup_signal_handlers();
    start_watchdog();
    
    while (1) {
        pause();
    }
}