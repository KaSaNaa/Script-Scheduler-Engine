#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "pthread.h"
// #include <sys/wait.h>
#include "scheduler.h"

#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#define access _access
#define F_OK 0
#else
#include <unistd.h>
#endif

#define MAX_TASKS 10
#define MAX_RETRIES 3

static Task task_queue[MAX_TASKS];
static int task_count = 0;

void execute_script(const char *script) { // this will execute a bash or python script depending on the system
    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        // printf("Current working dir: %s\n", cwd);
        printf("\n");
    } else {
        perror("getcwd() error");
    }

    if (access(script, F_OK) != -1){
        // printf("Script file exists: %s\n", script);
        printf("\n");
    } else {
        // printf("Script file doesnt not exist: %s\n", script);
        printf("\n");
        return;
    }

    char command[256];

    const char *ext = strrchr(script, '.');
    
    if (ext != NULL) {
        if (strcmp(ext, ".sh") == 0) {
            snprintf(command, sizeof(command), "bash %s", script);
        } else if (strcmp(ext, ".py") == 0) {
            snprintf(command, sizeof(command), "python %s", script);
        } else {
            printf("Unsupported script type: %s\n", ext);
            return;
        }
    } else {
        printf("No file extension found in script: %s\n", script);
        return;
    }
    printf("Executing command: %s\n", command);
    system(command);
}

void log_error(const char *message) {
    FILE * log_file = fopen("logs/scheduler.log", "a");
    if (log_file)
    {
        fprintf(log_file, "ERROR: %s: %s\n", message, strerror(errno));
        fclose(log_file);
    }
}

void execute_script_with_retries(const char *script) {
    int retries = 0;
    while(retries < MAX_RETRIES) {
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
    for (int i = 0; i < task_count; i++) {
        pthread_create(&threads[i], NULL, execute_script_thread, (void *)task_queue[i].script_name);
    }
    for (int i = 0; i < task_count; i++) {
        pthread_join(threads[i], NULL);
    }
}