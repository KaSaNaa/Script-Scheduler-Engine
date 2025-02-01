#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdbool.h>
#include <time.h>

typedef struct {
  char *script_name;
  time_t scheduled_time;
} Task;

typedef struct {
  Task *tasks;
  int capacity;
  int size;
} PriorityQueue;

PriorityQueue *create_priority_queue(int capacity);
void destroy_priority_queue(PriorityQueue *pq);
bool insert_task(PriorityQueue *pq, Task task);
Task extract_max(PriorityQueue *pq);
bool is_empty(PriorityQueue *pq);

#endif // PRIORITY_QUEUE_H