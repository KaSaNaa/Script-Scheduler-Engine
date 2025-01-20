To make the script scheduling engine robust and resilient, you can implement several features and mechanisms. Here are some suggestions:

```mermaid
graph TD
    A[Start] -->|Initialize scheduler| B[initialize_scheduler]
    B -->|Start scheduler thread| C[start_scheduler_thread]
    C -->|Create Task 1| D[Add Task 1]
    D -->|Create Task 2| E[Add Task 2]
    E -->|Setup signal handlers| F[setup_signal_handlers]
    F -->|Start watchdog process| G[start_watchdog]
    G -->|Fork watchdog process| H[Watchdog Process]
    H -->|Fork scheduler process| I[Scheduler Process]
    I -->|Run scheduler concurrently| J[run_scheduler_concurrent]
    J -->|Execute Script 1| K[Execute script1.py]
    J -->|Execute Script 2| L[Execute script2.sh]
    K -->|Log error if Script 1 fails| M[Log Error]
    L -->|Log error if Script 2 fails| N[Log Error]
    M -->|Retry Script 1 if fails| O[Retry Script 1]
    N -->|Retry Script 2 if fails| P[Retry Script 2]
    O -->|Max retries reached for Script 1| Q[Max Retries Reached]
    P -->|Max retries reached for Script 2| R[Max Retries Reached]
    Q -->|Log error for Script 1| S[Log Error]
    R -->|Log error for Script 2| T[Log Error]
    S -->|Scheduler process ends| U[Scheduler Process Ends]
    T -->|Scheduler process ends| U
    U -->|Watchdog restarts scheduler| V[Watchdog Restarts Scheduler]
    V -->|Fork scheduler process| I
    H -->|Watchdog monitors scheduler| W[Watchdog Monitors Scheduler]
    W -->|Fork scheduler process| H
    G -->|Pause main process| X[Main Process Pauses]
```

### 1. **Error Handling and Logging**
Implement detailed logging and error handling to capture and log all activities, errors, and exceptions.

#### Example:
```c
#include <errno.h>
#include <string.h>

void log_error(const char *message) {
    FILE *log_file = fopen("scheduler.log", "a");
    if (log_file) {
        fprintf(log_file, "ERROR: %s: %s\n", message, strerror(errno));
        fclose(log_file);
    }
}

void execute_script(const char *script) {
    // Existing code...
    if (system(command) != 0) {
        log_error("Failed to execute script");
    }
}
```

### 2. **Retry Mechanism**
Implement a retry mechanism for failed scripts to ensure they are re-executed a certain number of times before giving up.

#### Example:
```c
#define MAX_RETRIES 3

void execute_script_with_retries(const char *script) {
    int retries = 0;
    while (retries < MAX_RETRIES) {
        if (system(command) == 0) {
            return; // Success
        }
        retries++;
        log_error("Retrying script execution");
    }
    log_error("Max retries reached, giving up");
}
```

### 3. **Watchdog Process**
Implement a watchdog process to monitor the scheduler and restart it if it crashes.

#### Example:
```c
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void start_watchdog() {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process: Watchdog
        while (1) {
            int status;
            pid_t scheduler_pid = fork();
            if (scheduler_pid == 0) {
                // Child process: Scheduler
                run_scheduler();
                exit(0);
            }
            waitpid(scheduler_pid, &status, 0);
            log_error("Scheduler crashed, restarting...");
        }
    }
}
```

### 4. **Timeout Mechanism**
Implement a timeout mechanism to terminate unresponsive scripts.

#### Example:
```c
#include <signal.h>
#include <sys/time.h>

void timeout_handler(int signum) {
    log_error("Script execution timed out");
    exit(1);
}

void execute_script_with_timeout(const char *script, int timeout) {
    struct itimerval timer;
    signal(SIGALRM, timeout_handler);
    timer.it_value.tv_sec = timeout;
    timer.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);

    if (system(command) != 0) {
        log_error("Failed to execute script");
    }

    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);
}
```

### 5. **Concurrency**
Enable concurrent execution of multiple scripts using threading or multiprocessing.

#### Example:
```c
#include <pthread.h>

void *execute_script_thread(void *arg) {
    char *script = (char *)arg;
    execute_script(script);
    return NULL;
}

void run_scheduler_concurrent(void) {
    pthread_t threads[MAX_TASKS];
    for (int i = 0; i < task_count; i++) {
        pthread_create(&threads[i], NULL, execute_script_thread, (void *)task_queue[i].script_name);
    }
    for (int i = 0; i < task_count; i++) {
        pthread_join(threads[i], NULL);
    }
}
```

### 6. **Graceful Shutdown**
Implement a mechanism to handle graceful shutdowns and clean up resources.

#### Example:
```c
#include <signal.h>

void handle_shutdown(int signum) {
    log_error("Scheduler shutting down gracefully");
    // Clean up resources
    exit(0);
}

void setup_signal_handlers() {
    signal(SIGINT, handle_shutdown);
    signal(SIGTERM, handle_shutdown);
}
```

### Integration
Integrate these features into your scheduler:

#### 

scheduler.c


```c
#include "scheduler.h"
#include "func.h"
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_TASKS 10
#define MAX_RETRIES 3

static Task task_queue[MAX_TASKS];
static int task_count = 0;

void log_error(const char *message) {
    FILE *log_file = fopen("scheduler.log", "a");
    if (log_file) {
        fprintf(log_file, "ERROR: %s: %s\n", message, strerror(errno));
        fclose(log_file);
    }
}

void execute_script_with_retries(const char *script) {
    int retries = 0;
    while (retries < MAX_RETRIES) {
        if (system(script) == 0) {
            return; // Success
        }
        retries++;
        log_error("Retrying script execution");
    }
    log_error("Max retries reached, giving up");
}

void *execute_script_thread(void *arg) {
    char *script = (char *)arg;
    execute_script_with_retries(script);
    return NULL;
}

void run_scheduler_concurrent(void) {
    pthread_t threads[MAX_TASKS];
    for (int i = 0; i < task_count; i++) {
        pthread_create(&threads[i], NULL, execute_script_thread, (void *)task_queue[i].script_name);
    }
    for (int i = 0; i < task_count; i++) {
        pthread_join(threads[i], NULL);
    }
}

void handle_shutdown(int signum) {
    log_error("Scheduler shutting down gracefully");
    // Clean up resources
    exit(0);
}

void setup_signal_handlers() {
    signal(SIGINT, handle_shutdown);
    signal(SIGTERM, handle_shutdown);
}

void initialize_scheduler(void) {
    setup_signal_handlers();
    printf("Scheduler Initialized.\n");
}

void run_scheduler(void) {
    printf("Scheduler Running.\n");
    run_scheduler_concurrent();
}

void start_watchdog() {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process: Watchdog
        while (1) {
            int status;
            pid_t scheduler_pid = fork();
            if (scheduler_pid == 0) {
                // Child process: Scheduler
                run_scheduler();
                exit(0);
            }
            waitpid(scheduler_pid, &status, 0);
            log_error("Scheduler crashed, restarting...");
        }
    }
}
```

By implementing these features, you can make your script scheduling engine more robust and resilient, ensuring it runs continuously and handles various failure scenarios gracefully.
