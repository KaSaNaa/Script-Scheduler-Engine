#include "scheduler.h"
#include "func.h"
#include "priority_queue.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <limits.h>

#define CONFIG_FILE "config.json"
#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))

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
    char scheduled_time_str[20];
    strftime(scheduled_time_str, sizeof(scheduled_time_str),
             "%Y-%m-%d %H:%M:%S", localtime(&task.scheduled_time));
    printf("Task added: %s at %s\n", task.script_name,
           scheduled_time_str); // Debugging statement
    pthread_cond_signal(&queue_cond);
  }
  pthread_mutex_unlock(&queue_mutex);
  return success;
}

void *watch_config_file(void *arg) {
  (void)arg; // Unused parameter
  int fd = inotify_init();
  if (fd < 0) {
    log_error("inotify_init");
    return NULL;
  }

  int wd = inotify_add_watch(fd, CONFIG_FILE, IN_MODIFY);
  if (wd == -1) {
    log_error("inotify_add_watch");
    close(fd);
    return NULL;
  }

  char buffer[EVENT_BUF_LEN];
  while (1) {
    int length = read(fd, buffer, EVENT_BUF_LEN);
    if (length < 0) {
      log_error("read");
      break;
    }

    for (int i = 0; i < length; i += EVENT_SIZE + ((struct inotify_event *)&buffer[i])->len) {
      struct inotify_event *event = (struct inotify_event *)&buffer[i];
      if (event->mask & IN_MODIFY) {
        printf("Config file modified, reloading tasks...\n");
        load_tasks_from_json(CONFIG_FILE);
      }
    }
  }

  inotify_rm_watch(fd, wd);
  close(fd);
  return NULL;
}

void start_config_watcher(void) {
  pthread_t watcher_thread;
  pthread_create(&watcher_thread, NULL, watch_config_file, NULL);
  pthread_detach(watcher_thread);
}