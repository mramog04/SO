#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <string.h>

char logFilePath[] = "./log.txt";
/* FILE *logFile; */

typedef struct {
    int id;
    int estado;
}Cliente;


Cliente clientes[20];

pthread_mutex_t sem_logs;
pthread_mutex_t sem_clientes;
/* pthread_mutex_t sem_clientes[20]; */
pthread_cond_t cond_repo;
pthread_mutex_t mutex_repo;
pthread_mutex_t mutex_numClientes;

int numClientes = 0;

void handler(int sig);



int main() {

    struct sigaction sa,sa_ignore;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    sigaction(SIGUSR1, &sa, NULL);

    memset(&sa_ignore, 0, sizeof(sa_ignore));
    sa_ignore.sa_handler = SIG_IGN;


    pthread_mutex_init(&sem_clientes, NULL);
    /* for(int i = 0; i < 20; i++) {
        pthread_mutex_init(&sem_clientes[i], NULL);
    } */

    pthread_cond_init(&cond_repo, NULL);
    pthread_mutex_init(&mutex_repo, NULL);
    pthread_mutex_init(&sem_logs, NULL);
    pthread_mutex_init(&mutex_numClientes, NULL);

    /* logFile = fopen(logFilePath, "a");

    if(logFile == NULL){
        printf("Error al abrir el archivo de logs\n");
        exit(0);
    } */

    for (int i = 0; i < 20; i++) {
        pthread_mutex_lock(&sem_clientes[i]);
        clientes[i].id = -1;
        clientes[i].estado = -1;
        pthread_mutex_unlock(&sem_clientes[i]);
    }

    pthread_create(/* cajero1 */);
    pthread_create(/* cajero2 */);
    pthread_create(/* cajero3 */);

    pthread_create(/* reponedor */);

    pause();
    
}

void writeLogMessage(char *id, char *msg) {
    // Calculamos la hora actual
    printf("Escribiendo en el log\n");
    time_t now = time(0);
    struct tm *tlocal = localtime(&now);
    char stnow[25];
    strftime(stnow, 25, "%d/%m/%y %H:%M:%S", tlocal);
    
    // Escribimos en el log
    FILE *logFile = fopen(logFilePath, "a");
    fprintf(logFile, "[%s] %s: %s\n", stnow, id, msg);
    fclose(logFile);

    printf("se acabo de escribir en el log\n");
}


void handler(int sig){
    pthread_mutex_lock(&sem_clientes);
    Cliente cliente;
    for (int i = 0; i < 20; i++) {
        pthread_mutex_lock(&sem_clientes[i]);
        if (clientes[i].estado == -1) {
            cliente[i].estado = 0;
            pthread_mutex_lock(&mutex_numClientes);
            cliente[i].id = numClientes;
            numClientes++;
            pthread_mutex_unlock(&mutex_numClientes);
            cliente = clientes[i];
            break;
        }
    }
    pthread_mutex_unlock(&sem_clientes);

    return NULL;
}

void *cajero(void *args){

    Cliente cliente;

    int menorId = -1;
    int menorIndice = -1;

    pthread_mutex_lock(&sem_clientes);
    for (int i = 0; i < 20; i++) {
        if (clientes[i].id != -1 && (menorId == -1 || clientes[i].id < menorId)) {
            menorId = clientes[i].id;
            menorIndice = i;
        }
    }

    if (menorIndice != -1) {
        printf("Cliente con menor id: %d\n", clientes[menorIndice].id);
        cliente = clientes[menorIndice];
        cliente.estado = 1;
    }

    pthread_mutex_unlock(&sem_clientes);

    int tiempoCaja = (rand() % 5) + 1;
    
    pthread_mutex_lock(&sem_logs);
    writeLogMessage(cliente.id, "Hora de la atencion de la compra");
    pthread_mutex_unlock(&sem_logs);
    
    sleep(tiempoCaja);

    int numAleatorio = (rand() % 100) + 1;
    if(numAleatorio > 70 && numAleatorio < 96){
        pthread_mutex_lock(&mutex_repo);
        pthread_cond_signal(&cond_repo);
        pthread_mutex_unlock(&mutex_repo);
    }else if(numAleatorio > 95){
        pthread_mutex_lock(&sem_logs);
        writeLogMessage(cliente.id, "Problema con el pago");
        pthread_mutex_unlock(&sem_logs);
    }

    int precioCompra = (rand() % 100) + 1;
    pthread_mutex_lock(&sem_logs);
    writeLogMessage(cliente.id, "Compra realizada por un precio de : %d", precioCompra);
    pthread_mutex_unlock(&sem_logs);

    cliente.estado = 2;

    /* Implementar lo del descanso */
    
}