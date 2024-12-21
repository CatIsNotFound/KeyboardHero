#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

void *n = 0;
int is_count = 0;

struct Timer {
    time_t start, end;
};

// 开始计时
void start_count(struct Timer *timer) {
    timer->start = time(NULL);
}

long long stop_count(struct Timer *timer) {
    timer->end = time(NULL);
    return (timer->end - timer->start);
}


void sleep(int ms) {
    clock_t now, then;
    now = then = clock();
    while ((now - then) < ms) {
        now = clock();
    }
}

void* print(void* num) {
    printf("PID %d is running...\n", (int)num);
    printf("Wait for %dms.\n", (int)num * 100);
    sleep((int)num * 100);
    n = num;
    pthread_exit(NULL);
}


int main() {
    struct Timer timer;
    start_count(&timer);
    pthread_t pthread[3];
    srand(time(0));
    int f = 1;
    while (f--) {
        pthread_create(&pthread[0], NULL, print, (void *) (rand() % 10 + 1));
        pthread_create(&pthread[1], NULL, print, (void *) (rand() % 10 + 1));
        pthread_join(pthread[0], NULL);
        pthread_join(pthread[1], NULL);
        pthread_create(&pthread[0], NULL, print, (void *) (rand() % 10 + 1));
        pthread_create(&pthread[1], NULL, print, (void *) (rand() % 10 + 1));
        pthread_join(pthread[0], NULL);
        pthread_join(pthread[1], NULL);
    }
    printf("Runned: %d\n", n);
    printf("Runned for %llds.\n", stop_count(&timer));
    return 0;
}