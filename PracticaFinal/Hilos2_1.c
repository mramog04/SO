#include <pthread.h>
#include <stdio.h>

void *Hilo(void *arg) {
    printf(" %s\n", (char *)arg);
    pthread_exit(NULL);
}

int main(){
    pthread_t t1, t2;
    pthread_create(&t1, NULL, Hilo, "Ejecuta un hilo");
    pthread_create(&t2, NULL, Hilo, "Ejecuta otro hilo");
    printf("Fin del thread principal\n");
    return 0;
}