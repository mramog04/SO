#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>


void *hiloColgado(void *ptr) {
    while (1) {
        printf(" %s: PID= %d\n", (char *)ptr, getpid());
        sleep(1);
    }
}

void *hiloAsesino(void *ptr) {
    printf(" %s: PID= %d. Voy a esperar 10 segundos y termino todo este lío.\n", (char *)ptr, getpid());
    sleep(10);
    exit(0);
}

int main(){
    pthread_t thread1, thread2;
    char *message1 = "Hilo colgado";
    char *message2 = "Hilo asesino de proceso";

    pthread_create(&thread1, NULL, hiloColgado, (void*) message1);
    pthread_create(&thread2, NULL, hiloAsesino, (void*) message2);

    while (1) {
        printf("Hilo principal: PID= %d\n", getpid());
        sleep(1);
    }

    return 0;
}