#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdbool.h>

typedef struct {
    char *script_name;
    int priority;
} Task;

typedef struct {
    Task *tasks;
    int capacity;
    int size;
} PriorityQueue;

PriorityQueue* create_priority_queue(int capacity);
void destroy_priority_queue(PriorityQueue *pq);
bool insert_task(PriorityQueue *pq, Task task);
Task extract_max(PriorityQueue *pq);
bool is_empty(PriorityQueue *pq);

#endif // PRIORITY_QUEUE_H