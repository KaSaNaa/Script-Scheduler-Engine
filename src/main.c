#include <json-c/json_object.h>
#include <json-c/json_object_iterator.h>
#include <json-c/json_tokener.h>
#include <scheduler.h>
#include <func.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <json-c/json.h>
#include <stdio.h>

void load_tasks_from_json(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file)
    {
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
    size_t n_tasks;
    size_t i;

    parsed_json = json_tokener_parse(data);
    json_object_object_get_ex(parsed_json, "tasks", &tasks);
    n_tasks = json_object_array_length(tasks);

    for (i = 0; i < n_tasks; i++) {
        task = json_object_array_get_idx(tasks, i);
        json_object_object_get_ex(task, "script", &script);
        json_object_object_get_ex(task, "priority", &priority);

        Task new_task = {
            .script_name = strdup(json_object_get_string(script)),
            .priority = json_object_get_int(priority)
        };
        add_task(new_task);
    }

    free(data);
    json_object_put(parsed_json);
}

int main(void) {
    initialize_scheduler();
    start_scheduler_thread();

    load_tasks_from_json("config.json");

    setup_signal_handlers();
    start_watchdog();
    
    while (1) {
        pause();
    }
}