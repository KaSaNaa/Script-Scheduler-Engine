#include <asm-generic/errno.h>
#include <priority_queue.h>
#include <stdbool.h>
#include <stdlib.h>

PriorityQueue *create_priority_queue(int capacity) {
  PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
  pq->tasks = (Task *)malloc(capacity * sizeof(Task));
  pq->capacity = capacity;
  pq->size = 0;
  return pq;
}

void destroy_priotiy_queue(PriorityQueue *pq) {
  free(pq->tasks);
  free(pq);
}

void swap(Task *a, Task *b) {
  Task temp = *a;
  *a = *b;
  *b = temp;
}

void heapify_up(PriorityQueue *pq, int index) {
  while (index > 0) {
    int parent = (index - 1) / 2;
    if (pq->tasks[index].priority > pq->tasks[parent].priority) {
      swap(&pq->tasks[index], &pq->tasks[parent]);
      index = parent;
    } else {
      break;
    }
  }
}

void heapify_down(PriorityQueue *pq, int index) {
  int left, right, largest;
  while (1) {
    left = 2 * index + 1;
    right = 2 * index + 2;
    largest = index;

    if (left < pq->size &&
        pq->tasks[left].priority > pq->tasks[largest].priority) {
      largest = left;
    }
    if (right < pq->size &&
        pq->tasks[right].priority > pq->tasks[largest].priority) {
      largest = right;
    }
    if (largest != index) {
      swap(&pq->tasks[index], &pq->tasks[largest]);
      index = largest;
    } else {
      break;
    }
  }
}

bool insert_task(PriorityQueue *pq, Task task) {
  if (pq->size >= pq->capacity) {
    return false;
  }
  pq->tasks[pq->size] = task;
  heapify_up(pq, pq->size);
  pq->size++;
  return true;
}

Task extract_max(PriorityQueue *pq) {
  Task max_task = pq->tasks[0];
  pq->tasks[0] = pq->tasks[pq->size - 1];
  pq->size--;
  heapify_down(pq, 0);
  return max_task;
}

bool is_empty(PriorityQueue *pq) { return pq->size == 0; }