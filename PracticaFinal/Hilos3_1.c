#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *Hilo1(void *arg) {
    pthread_mutex_lock(arg);
    printf("Comienza un hilo\n");
    sleep(5);
    printf("Finaliza un hilo\n");
    pthread_mutex_unlock(arg);
    pthread_exit(NULL);
} /* Fin de Hilo */

int main(){
    pthread_t t1, t2;
    pthread_mutex_t misemaforo;

    // Creamos un semaforo
    if (pthread_mutex_init(&misemaforo, NULL) != 0)
        exit(-1);

    pthread_create(&t1, NULL, Hilo1, &misemaforo);
    pthread_create(&t2, NULL, Hilo1, &misemaforo);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    if (pthread_mutex_destroy(&misemaforo) != 0)
        exit(-1);

    return 0;
}