#include <scheduler.h>
#include <func.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

Task task_queue[MAX_TASKS];
int task_count = 0;
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_cond = PTHREAD_COND_INITIALIZER;

void initialize_scheduler(void) {
    // Initialize the scheduler (setup necessary data structures, etc.)
    printf("Scheduler Initialized.\n");
}

void *scheduler_thread_func(void *arg) {
    (void)arg; // Unused parameter
    run_scheduler_concurrent();
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