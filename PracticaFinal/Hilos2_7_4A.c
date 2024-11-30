#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#define N 15.0

void *tareaHilo(void *ptr) {
    int x;
    printf("COMIENZO TAREA HILO %d\n", *(int *)ptr);
    srand((int)pthread_self());
    x = 1 + (int)(N * rand() / RAND_MAX + 1.0); // X es un número aleatorio entre 1 y N
    sleep(x);
    printf("FIN TAREA HILO %d\n", *(int *)ptr);
    return NULL;
}

int main(){
    pthread_t thread1;
    int hilo = 1;
    pthread_create(&thread1, NULL, tareaHilo, &hilo);
    pthread_join(thread1, NULL);
    sleep(1);
    hilo++;
    tareaHilo(&hilo);
    pthread_exit(NULL);
    return 0;
}