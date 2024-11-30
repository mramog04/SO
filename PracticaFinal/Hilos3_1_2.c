#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#define N 2.0

pthread_mutex_t mutex;
int i[5] = {1, 2, 3, 4, 5};

void *hilo(void *ptr) {
    int x;
    pthread_mutex_lock(&mutex);
    printf("COMIENZO TAREA HILO %d\n", *(int *)ptr);
    srand((int)pthread_self());
    x = 1 + (int)(N * rand() / RAND_MAX + 1.0); // X es un número aleatorio entre 1 y N
    sleep(x);
    printf("FIN TAREA HILO %d: Tiempo ejecución %d\n", *(int *)ptr, x);
    pthread_mutex_unlock(&mutex);
}

int main(){
    pthread_t t1, t2, t3, t4, t5;
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&t1, NULL, hilo, (void *)&i[0]);
    pthread_create(&t2, NULL, hilo, (void *)&i[1]);
    pthread_create(&t3, NULL, hilo, (void *)&i[2]);
    pthread_create(&t4, NULL, hilo, (void *)&i[3]);
    pthread_create(&t5, NULL, hilo, (void *)&i[4]);
    pthread_exit(NULL);
    return 0;
}