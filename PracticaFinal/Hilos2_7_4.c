#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

void *hilo(void *ptr) {
    int i, x = *(int*)ptr;
    for (i = 0; i < x; i++) {
        printf("Voy a repetir %d y estoy en la iteración %d\n", x, i);
        sleep(1);
    }
}

int main(){
    pthread_t thread1, thread2;
    int n1 = 5, n2 = 10;
    pthread_create(&thread1, NULL, hilo, (void*)&n1);
    pthread_create(&thread2, NULL, hilo, (void*)&n2);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL); 
    return 0;
}