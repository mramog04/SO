#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <string.h>

void handler(int sig);
void *hcajero1(void *args);
void *hcajero2(void *args);
void *hcajero3(void *args);
void *cliente(void *args);
void *hreponedor(void *args);
void writeLogMesage(char *id,char *msg);

char logFilePath[] = "./log.txt";

typedef struct {
    int id;
    int estado;
    int posCliente;
}Cliente;

Cliente clientes[20];

pthread_mutex_t mutex_logs;
pthread_mutex_t mutex_clientes;
pthread_mutex_t mutex_repo;
pthread_mutex_t mutex_numClientes;
pthread_mutex_t mutex_Ccajero1, mutex_Ccajero2, mutex_Ccajero3;
pthread_cond_t cond_repo;

int numClientes = 0;

int Ccajero1 = 0, Ccajero2 = 0, Ccajero3 = 0;

int main(){
    pthread_t cajero1, cajero2, cajero3, reponedor;
    pthread_t hilos_clientes[20];

    struct sigaction sa,sa_ignore;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    sigaction(SIGUSR1,&sa,NULL);

    memset(&sa_ignore,0,sizeof(sa_ignore));
    sa_ignore.sa_handler = SIG_IGN;

    pthread_mutex_init(&mutex_clientes,NULL);
    pthread_mutex_init(&mutex_logs,NULL);
    pthread_mutex_init(&mutex_repo,NULL);
    pthread_mutex_init(&mutex_numClientes,NULL);
    pthread_mutex_init(&mutex_Ccajero1,NULL);
    pthread_mutex_init(&mutex_Ccajero2,NULL);
    pthread_mutex_init(&mutex_Ccajero3,NULL);

    for(int i = 0;i < 20; i++){
        pthread_mutex_lock(&mutex_clientes);
        clientes[i].id = -1;
        clientes[i].estado = -1;
        clientes[i].posCliente = -1;
        pthread_create(hilos_clientes[i],NULL,cliente,&clientes[i]);
        pthread_mutex_unlock(&mutex_clientes);
    }

    int errorCreateRepo = pthread_create(&reponedor,NULL,hreponedor,NULL);
    if(errorCreateRepo!=0){
        printf("Error al crear el hilo del reponedor.\n");
        exit(0);
    }

    int errorCreateCajero1 = pthread_create(&cajero1,NULL,hcajero1,NULL);
    if(errorCreateCajero1!=0){
        printf("Error al crear el hilo del cajero1.\n");
        exit(0);
    }

    int errorCreateCajero2 = pthread_create(&cajero2,NULL,hcajero2,NULL);
    if(errorCreateCajero2!=0){
        printf("Error al crear el hilo del cajero2.\n");
        exit(0);
    }

    int errorCreateCajero3 = pthrea_create(&cajero3,NULL,hcajero3,NULL);
    if(errorCreateCajero3!=0){
        printf("Error al crear el hilo del cajero3.\n");
        exit(0);
    }

    while(1){
        pause();
    }
}