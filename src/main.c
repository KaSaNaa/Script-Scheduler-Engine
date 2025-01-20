#include <stdio.h>
#include <pthread.h>
#include <threads.h>

pthread_mutex_t serial_mtx;

void *run(void *arg) {
    (void)arg;
    static int serial = 0; // Shared static variable!

    // Acquire the mutex--all threads will block on this call until
    // they get the lock

    pthread_mutex_lock(&serial_mtx);

    printf("Thread running! %d\n", serial);

    serial++;

    pthread_mutex_unlock(&serial_mtx);

    return NULL;
}

#define THREAD_COUNT 10

int main(void) {
    pthread_t t[THREAD_COUNT];

    // Initialzie the mutex variable, indicating this is a normal
    // no-frills, mutex:

    // mutex: short for “mutual exclusion”, AKA a “lock” on a section of code that only one thread is permitted to execute.

    pthread_mutex_init(&serial_mtx, NULL);

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(t + i, NULL, run, NULL);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(t[i], NULL);
    }

    pthread_mutex_destroy(&serial_mtx);
}

#include <stdio.h>

int main(void) {
    char s[] = "Hello World!";
    char *t;

    t = s;
    printf("%s %p\n", "print s = ", s);
    printf("%s %p\n %s %p\n", "t address = ", t, "s address", (void *)s);

    t[0] = 'z';
    printf("%s\n", s);
}