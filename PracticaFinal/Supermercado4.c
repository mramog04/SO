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
    int posClientes;
}Cliente;


Cliente clientes[20];

pthread_mutex_t sem_logs;
pthread_mutex_t sem_clientes;
/* pthread_mutex_t sem_clientes[20]; */
pthread_cond_t cond_repo;
pthread_mutex_t mutex_repo;
pthread_mutex_t mutex_numClientes;
pthread_mutex_t mutex_Ccajero1, mutex_Ccajero2, mutex_Ccajero3;

int numClientes = 0;

int Ccajero1 = 0, Ccajero2 = 0, Ccajero3 = 0;

void handler(int sig);
void *hcajero1(void *args);
void *hcajero2(void *args);
void *hcajero3(void *args);
void *cliente(void *args);
void *hreponedor(void *prc);
void writeLogMessage(char *id, char *msg);




int main() {

    pthread_t cajero1, cajero2, cajero3, reponedor;

    pthread_t hilos_clientes[20];

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
    pthread_mutex_init(&mutex_Ccajero1, NULL);
    pthread_mutex_init(&mutex_Ccajero2, NULL);
    pthread_mutex_init(&mutex_Ccajero3, NULL);

    /* logFile = fopen(logFilePath, "a");

    if(logFile == NULL){
        printf("Error al abrir el archivo de logs\n");
        exit(0);
    } */

    for (int i = 0; i < 20; i++) {
        pthread_mutex_lock(&sem_clientes);
        clientes[i].id = -1;
        clientes[i].estado = -1;
        clientes[i].posClientes = i;
        pthread_create(hilos_clientes[i], NULL, cliente, &clientes[i]);
        pthread_mutex_unlock(&sem_clientes);
    }

    int errorCrearRepo = pthread_create(&reponedor, NULL, hreponedor, NULL);
    if(errorCrearRepo != 0){
        printf("Error al crear el hilo del reponedor\n");
        exit(0);
    }

    int errorCrearCajero1 = pthread_create(&cajero1, NULL, hcajero1, NULL);
    if(errorCrearCajero1 != 0){
        printf("Error al crear el hilo del cajero 1\n");
        exit(0);
    }

    int errorCrearCajero2 = pthread_create(&cajero2, NULL, hcajero2, NULL);
    if(errorCrearCajero2 != 0){
        printf("Error al crear el hilo del cajero 2\n");
        exit(0);
    }

    int errorCrearCajero3 = pthread_create(&cajero3, NULL, hcajero3, NULL);
    if(errorCrearCajero3 != 0){
        printf("Error al crear el hilo del cajero 3\n");
        exit(0);
    }


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

void *hcajero1(void *args){

    Cliente cliente;


    while (1) {
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
        if (numAleatorio > 70 && numAleatorio < 96) {
            pthread_mutex_lock(&mutex_repo);
            pthread_cond_signal(&cond_repo);
            pthread_mutex_unlock(&mutex_repo);
        } else if (numAleatorio > 95) {
            pthread_mutex_lock(&sem_logs);
            writeLogMessage(cliente.id, "Problema con el pago");
            pthread_mutex_unlock(&sem_logs);
            /* exit(0); */
        }

        int precioCompra = (rand() % 100) + 1;
        pthread_mutex_lock(&sem_logs);
        writeLogMessage(cliente.id, "Compra realizada por un precio de : %d", precioCompra);
        pthread_mutex_unlock(&sem_logs);

        cliente.estado = 2;

        pthread_mutex_lock(&mutex_Ccajero1);
        if (Ccajero1 == 20) {
            sleep(20);
            Ccajero1 = 0;
        } else {
            Ccajero1++;
        }
        pthread_mutex_unlock(&mutex_Ccajero1);
    }

    pthread_exit(NULL);
}

void *hcajero2(void *args){



    while (1) {
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
            /* exit(0); */
        }

        int precioCompra = (rand() % 100) + 1;
        pthread_mutex_lock(&sem_logs);
        writeLogMessage(cliente.id, "Compra realizada por un precio de : %d", precioCompra);
        pthread_mutex_unlock(&sem_logs);

        cliente.estado = 2;

        pthread_mutex_lock(&mutex_Ccajero2);
        if(Ccajero2==20){
            sleep(20);
            Ccajero2=0;
        }else{
            Ccajero2++;
        }
        pthread_mutex_unlock(&mutex_Ccajero2);
    }

    pthread_exit(NULL);
}

void *hcajero3(void *args){

    Cliente cliente;

    while (1) {
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
            /* exit(0); */
        }

        int precioCompra = (rand() % 100) + 1;
        pthread_mutex_lock(&sem_logs);
        writeLogMessage(cliente.id, "Compra realizada por un precio de : %d", precioCompra);
        pthread_mutex_unlock(&sem_logs);

        cliente.estado = 2;

        pthread_mutex_lock(&mutex_Ccajero3);
        if(Ccajero3==20){
            sleep(20);
            Ccajero3=0;
        }else{
            Ccajero3++;
        }
        pthread_mutex_unlock(&mutex_Ccajero3);
    }

    pthread_exit(NULL);
}

void cliente(void *args){

    Cliente cliente = *((Cliente *)args);

    while(clientes[i].estado==-1){
        sleep(3);
    }

    pthread_mutex_lock(&sem_logs);
    writeLogMessage(cliente.id,"Hora de llegada del cliente");
    pthread_mutex_unlock(&sem_logs);

    int tiempoCansado = (rand() % 35) + 20;
    sleep(tiempoCansado);
    if(cliente.estado==0){
        pthread_mutex_lock(&sem_logs);
        writeLogMessage(cliente.id,"El cliente se ha marchado sin ser atendido");
        pthread_mutex_unlock(&sem_logs);
        cliente.estado=2;
    }
    while (cliente.estado != 2) {
        sleep(2);
    }

    pthread_mutex_lock(&sem_logs);
    writeLogMessage(cliente.clienteid,"Finalizacion del cliente");
    pthread_mutex_unlock(&sem_logs);

    pthread_mutex_lock(&sem_clientes);
    pthread_mutex_lock(&mutex_numClientes);
    cliente.id = numClientes;
    numClientes++;
    pthread_mutex_unlock(&sem_clientes);
    pthread_mutex_unlock(&mutex_numClientes);


}

void *hreponedor(void *prc){
    while(1){
        pthread_mutex_lock(&mutex_repo);
        pthread_cond_wait(&cond_repo, &mutex_repo);
        printf("Reponedor está reponiendo productos...\n");
        sleep((rand() % 6) + 5);
        printf("Reponedor ha terminado de reponer productos.\n");
        pthread_mutex_unlock(&mutex_repo);
    }
    return NULL;
}

    