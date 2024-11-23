#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#define N 5.0
#include <unistd.h>

int sem=1;
void *hilo(void *ptr) {
    while(sem==1){
        sleep(5);
    }
int x;
sem = 1;
printf("COMIENZO TAREA HILO\n");
srand((int)pthread_self());
x=1+(int)(N*rand()/ RAND_MAX +1.0); // X es un número aleatorio entre 1 y N
sleep(x);
printf("FIN TAREA HILO : Tiempo ejecucion %d\n",x);
sem=0;
}
int main() {
pthread_t t1, t2, t3, t4, t5;
pthread_create(&t1, NULL, hilo, NULL);
pthread_create(&t2, NULL, hilo, NULL);
pthread_create(&t3, NULL, hilo, NULL);
pthread_create(&t4, NULL, hilo, NULL);
pthread_create(&t5, NULL, hilo, NULL);
printf("Van a comenzar los hilos :\n");
sem = 0;
pthread_exit(NULL);
return 0;
}