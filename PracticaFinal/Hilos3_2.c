#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#define N 5.0

pthread_mutex_t mutex;
int i[5] = {1, 2, 3, 4, 5};
pthread_cond_t condiciones[6];

/*
Cada condición expresa la finalización de un hilo
condiciones[0] fin del hilo main
condiciones[1] fin del hilo t1
condiciones[2] fin del hilo t2
condiciones[3] fin del hilo t3
condiciones[4] fin del hilo t4
condiciones[5] fin del hilo t5
*/

pthread_cond_t condicion; // han entrado los cinco threads
int M = 0; // Numero de threads que han entrado

void *hilo(void *ptr) {
    int x;
    pthread_mutex_lock(&mutex);
    // Enviar señal al thread main cuando los 5 threads estén en el semáforo
    if ((++M) >= 5) pthread_cond_signal(&condicion);
    // Comprobamos si ha finalizado su hilo precedente
    pthread_cond_wait(&condiciones[*(int *)ptr - 1], &mutex);
    printf("COMIENZO TAREA HILO %d\n", *(int *)ptr);
    srand((int)pthread_self());
    x = 1 + (int)(N * rand() / RAND_MAX + 1.0); // X es un número aleatorio entre 1 y N
    sleep(x);
    printf("FIN TAREA HILO %d: Tiempo ejecucion %d\n", *(int *)ptr, x);
    // enviamos señal de finalización del hilo
    pthread_cond_signal(&condiciones[*(int *)ptr]);
    pthread_mutex_unlock(&mutex);
}

int main(){
    pthread_t t1, t2, t3, t4, t5;
    pthread_attr_t tattr;
    pthread_mutex_init(&mutex, NULL);
    int x;
    // inicializamos las condiciones de espera de los hilos
    for (x = 0; x <= 5; x++)
        if (pthread_cond_init(&condiciones[x], NULL) != 0)
            exit(-1);
    // inicializamos la condición de espera del hilo principal
    if (pthread_cond_init(&condicion, NULL) != 0) exit(-1);
    pthread_mutex_lock(&mutex);
    pthread_create(&t1, NULL, hilo, (void *)&i[0]);
    pthread_create(&t2, NULL, hilo, (void *)&i[1]);
    pthread_create(&t3, NULL, hilo, (void *)&i[2]);
    pthread_create(&t4, NULL, hilo, (void *)&i[3]);
    pthread_create(&t5, NULL, hilo, (void *)&i[4]);
    // Esperamos a que los 5 threads queden detenidos en el semáforo
    pthread_cond_wait(&condicion, &mutex);
    printf("Van a comenzar los hilos\n");
    // Enviamos la señal de finalización del hilo principal
    pthread_cond_signal(&condiciones[0]);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
    return 0;
}