#include "pthread.h"
#include "scheduler.h"
#include <errno.h>
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
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    fprintf(log_file, "[%s] ERROR: %s: %s\n", timestamp, message, strerror(errno));
    fclose(log_file);
  } else {
    perror("Failed to open log file. Create the directory /log if it does not exist.");
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
        while (task_count == 0) {
            pthread_cond_wait(&queue_cond, &queue_mutex);
        }

        Task task = task_queue[0];
        for (int i = 1; i < task_count; i++) {
            task_queue[i - 1] = task_queue[i];
        }
        task_count--;

        pthread_mutex_unlock(&queue_mutex);

        pthread_create(&threads[0], NULL, execute_script_thread, (void *)task.script_name);
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