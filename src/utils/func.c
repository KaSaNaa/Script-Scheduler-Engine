#define __USE_XOPEN
#define _GNU_SOURCE
#define _XOPEN_SOURCE 700
#include "scheduler.h"
#include <errno.h>
#include <json-c/json.h>
#include <json-c/json_object.h>
#include <json-c/json_object_iterator.h>
#include <json-c/json_tokener.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define MAX_RETRIES 3

void execute_script(const char *script) {
  char cwd[256];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("Current working dir: %s\n", cwd);
  } else {
    perror("getcwd() error");
  }

  if (access(script, F_OK) != -1) {
    printf("Script file exists: %s\n", script);
  } else {
    printf("Script file does not exist: %s\n", script);
    return;
  }

  if (access(script, X_OK) != 0) {
    printf("Script file is not executable: %s\n", script);
    return;
  }

  char command[256];
  const char *ext = strrchr(script, '.');

  if (ext != NULL) {
    if (strcmp(ext, ".sh") == 0) {
      snprintf(command, sizeof(command), "bash %s", script);
    } else if (strcmp(ext, ".py") == 0) {
      snprintf(command, sizeof(command), "python3 %s", script);
    } else {
      printf("Unsupported script type: %s\n", ext);
      return;
    }
  } else {
    printf("No file extension found in script: %s\n", script);
    return;
  }

  printf("Executing command: %s\n", command);
  int result = system(command);
  if (result != 0) {
    printf("Command execution failed with code: %d\n", result);
  }
}

void log_error(const char *message) {
  FILE *log_file = fopen("logs/scheduler.log", "a");
  if (log_file) {
    time_t now = time(NULL);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S",
             localtime(&now));
    fprintf(log_file, "[%s] ERROR: %s: %s\n", timestamp, message,
            strerror(errno));
    fclose(log_file);
  } else {
    perror("Failed to open log file. Create the directory /log if it does not "
           "exist.");
  }
}

void execute_script_with_retries(const char *script) {
  int retries = 0;
  while (retries < MAX_RETRIES) {
    if (system(script) == 0) {
      return;
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
  while (1) {
    pthread_mutex_lock(&queue_mutex);
    while (is_empty(task_queue)) {
      pthread_cond_wait(&queue_cond, &queue_mutex);
    }

    time_t current_time = time(NULL);
    Task task = extract_max(task_queue);

    // Print the current time and the scheduled time for the task
    char current_time_str[20];
    char scheduled_time_str[20];
    strftime(current_time_str, sizeof(current_time_str), "%Y-%m-%d %H:%M:%S",
             localtime(&current_time));
    strftime(scheduled_time_str, sizeof(scheduled_time_str),
             "%Y-%m-%d %H:%M:%S", localtime(&task.scheduled_time));
    printf("\nCurrent time: %s\n", current_time_str);
    printf("\nScheduled time for task: %s\n\n", scheduled_time_str);

    double time_diff = difftime(task.scheduled_time, current_time);
    if (time_diff > 0) {
      // If the task is scheduled for the future, reinsert it and wait
      insert_task(task_queue, task);
      pthread_mutex_unlock(&queue_mutex);
      sleep((unsigned int)time_diff); // Sleep until the task is due
      continue;
    }

    pthread_mutex_unlock(&queue_mutex);

    pthread_create(&threads[0], NULL, execute_script_thread,
                   (void *)task.script_name);
    pthread_join(threads[0], NULL);
  }
}

void handle_shutdown(int signum) {
  (void)signum; // Unused parameter
  log_error("Scheduler shutting down gracefully..");
  exit(0);
}

void setup_signal_handlers() {
  signal(SIGINT, handle_shutdown);
  signal(SIGTERM, handle_shutdown);
}

void start_watchdog() {
  pid_t pid = fork();
  if (pid == 0) {
    // child process: watchdog
    while (1) {
      int status;
      pid_t scheduler_pid = fork();
      if (scheduler_pid == 0) {
        // Child process: Scheduler
        run_scheduler_concurrent();
        exit(0);
      }
      waitpid(scheduler_pid, &status, 0);
      log_error("Scheduler Crashed, restarting...");
    }
  }
}

void load_tasks_from_json(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Failed to open config.json");
    return;
  }

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *data = malloc(length + 1);
  fread(data, 1, length, file);
  fclose(file);
  data[length] = '\0';

  struct json_object *parsed_json;
  struct json_object *tasks;
  struct json_object *task;
  struct json_object *script;
  struct json_object *priority;
  struct json_object *time;
  size_t n_tasks;
  size_t i;
  struct tm tm;

  parsed_json = json_tokener_parse(data);
  json_object_object_get_ex(parsed_json, "tasks", &tasks);
  n_tasks = json_object_array_length(tasks);

  for (i = 0; i < n_tasks; i++) {
    task = json_object_array_get_idx(tasks, i);
    json_object_object_get_ex(task, "script", &script);
    json_object_object_get_ex(task, "priority", &priority);
    json_object_object_get_ex(task, "time", &time);

    const char *time_str = json_object_get_string(time);
    printf("\nParsing time: %s\n", time_str); // Debugging statement

    if (strptime(time_str, "%Y-%m-%dT%H:%M:%S", &tm) == NULL) {
      printf("Failed to parse time: %s\n", time_str); // Debugging statement
    }
  }
  time_t scheduled_time = mktime(&tm);

  Task new_task = {.script_name = strdup(json_object_get_string(script)),
                   .priority = json_object_get_int(priority),
                   .scheduled_time = scheduled_time};
  add_task(new_task);

  free(data);
  json_object_put(parsed_json);
}