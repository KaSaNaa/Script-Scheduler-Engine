#include "scheduler.h"
#include "func.h"
#include "priority_queue.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

PriorityQueue *task_queue;
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_cond = PTHREAD_COND_INITIALIZER;

void initialize_scheduler(void) {
  task_queue = create_priority_queue(MAX_TASKS);
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
  bool success = insert_task(task_queue, task);
  if (success) {
    pthread_cond_signal(&queue_cond);
  }
  pthread_mutex_unlock(&queue_mutex);
  return success;
}