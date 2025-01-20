#include <scheduler.h>
#include <func.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_TASKS 10

static Task task_queue[MAX_TASKS];
static int task_count = 0;
static pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t queue_cond = PTHREAD_COND_INITIALIZER;

void initialize_scheduler(void) {
    // Initialize the scheduler (setup necessary data structures, etc.)
    printf("Scheduler Initialized.\n");
}

void *scheduler_thread_func(void *arg) {
    (void)arg; // Unused parameter
    while (1) {
        pthread_mutex_lock(&queue_mutex);
        while (task_count == 0) {
            pthread_cond_wait(&queue_cond, &queue_mutex);
        }

        Task task = task_queue[0];
        for (int i = 1; i < task_count; i++) {
            task_queue[i - 1] = task_queue[i];
        }
        task_count--;

        pthread_mutex_unlock(&queue_mutex);

        printf("Executing script: %s\n", task.script_name);
        execute_script(task.script_name);
    }
    return NULL;
}

void start_scheduler_thread(void) {
    pthread_t scheduler_thread;
    pthread_create(&scheduler_thread, NULL, scheduler_thread_func, NULL);
    pthread_detach(scheduler_thread);
}

bool add_task(Task task) {
    pthread_mutex_lock(&queue_mutex);
    if (task_count >= MAX_TASKS) {
        pthread_mutex_unlock(&queue_mutex);
        return false;
    }
    task_queue[task_count++] = task;
    pthread_cond_signal(&queue_cond);
    pthread_mutex_unlock(&queue_mutex);
    return true;
}