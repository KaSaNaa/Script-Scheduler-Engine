#include <stdio.h>
#include <pthread.h>

void *run(void *arg) {
    (void)arg;
    static int serial = 0;

    printf("Thread running! %d\n", serial);

    serial++;
    return NULL;
}

#define THREAD_COUNT 10

int main(void) {
    pthread_t t[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(t + i, NULL, run, NULL);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(t[i], NULL);
    }
}