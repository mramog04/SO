#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <string.h>



#define N 5.0

void handler(int sig);
int clientes_lleno();
int cliente_libre();
void iniciarCliente(void *prc);
void *cliente(void *args);
void *caja_1(void *prc);
void *caja_2(void *prc);
void *caja_3(void *prc);
void *reponer(void *prc);
void writeLogMessage(char *id, char *msg);

typedef struct {
    int cliente_id;
    char nombre[50];
    int precio_compra;
    int tiempo_caja;
    int problema_precio;
    int problema_pagar;
    int tiempo_cansado;
} Cliente;

typedef struct {
    int numCliente;
} argsHiloCliente;

pthread_mutex_t mutex_1,mutex_2,mutex_3,mutex_repo,mutex_log;
pthread_mutex_t sem_clientes[20];
pthread_cond_t cond_repo;
struct sigaction sa,sa_ignore;

int clientesCaja1 = 0;
int clientesCaja2 = 0;
int clientesCaja3 = 0;

int ingresosCaja1 = 0;
int ingresosCaja2 = 0;
int ingresosCaja3 = 0;


/* char logFile[] = "./log.txt"; */
char logFileName[] = "./log.txt";  

Cliente clientes[20];
pthread_t hilos_clientes[20];
int num_clientes = 0;

void iniciarCliente(void *prc){
    Cliente* cliente = (Cliente*)prc;
    cliente->cliente_id = -1;
    strcpy(cliente->nombre, "");
    cliente->precio_compra = 0;
    cliente->tiempo_caja = 0;
    cliente->problema_precio = 0;
    cliente->problema_pagar = 0;
    cliente->tiempo_cansado = 0;
}


void handler(int sig) {
    sigaction(SIGUSR1, &sa_ignore, NULL);
    if(sig == SIGUSR1) {
        if(clientes_lleno()==0){
            pthread_mutex_lock(&mutex_log);
            Cliente* new_cliente = malloc(sizeof(Cliente));
            argsHiloCliente* args = (argsHiloCliente*)malloc(sizeof(argsHiloCliente));
            new_cliente->cliente_id = num_clientes+1;
            /* new_cliente->nombre = strcpy("cliente_%d", num_clientes+1); */
            sprintf(new_cliente->nombre, "cliente_%d", num_clientes+1);
            new_cliente->precio_compra = (rand() % 100) + 1;
            new_cliente->tiempo_caja = (rand() % 5) + 1;
            if(((rand() % 100)+1)>90){
                new_cliente->tiempo_cansado = 1;
            } else {
                new_cliente->tiempo_cansado = 0;
            } 
            if(((rand() % 100)+1)>95){
                new_cliente->problema_precio = 1;
            }else{
                new_cliente->problema_precio = 0;
            }
            if(((rand() % 100)+1)>75){
                new_cliente->problema_pagar = 1;
            }else{
                new_cliente->problema_pagar = 0;
            }
            for(int i = 0; i < 20; i++) {
                if(clientes[i].cliente_id == -1) {
                    clientes[i] = *new_cliente;
                    args->numCliente = i;
                    pthread_create(&hilos_clientes[i], NULL, cliente, args);
                    num_clientes++;
                    free(args);
                    break;
                }
            }
            writeLogMessage(new_cliente->nombre, "Nuevo cliente en la cola");
            pthread_mutex_unlock(&mutex_log);
            printf("Nuevo cliente: %s\n", new_cliente->nombre);
            sigaction(SIGUSR1, &sa, NULL);
            free(args);
        }
    }
}

int clientes_lleno(){
    for(int i = 0; i < 20; i++) {
        if(clientes[i].cliente_id == -1) {
            return 0;
        }
    }
    return 1;
}

int cliente_libre(){
    int x = -1;
    for(int i = 0; i < 20; i++) {
        if(clientes[i].cliente_id != -1) {
            if(clientes[i].cliente_id < x){
                x = clientes[i].cliente_id;
            }
        }
    }
    return x;
}

void *cliente(void *args){
    argsHiloCliente* args2 = (argsHiloCliente*)args;
    int numCliente = args2->numCliente;
    Cliente* cliente = &clientes[numCliente];
    if(cliente->tiempo_cansado == 1) {
        sleep(10);
        pthread_mutex_lock(&sem_clientes[cliente->cliente_id]);
        if(clientes[cliente->cliente_id].cliente_id == -1) {
            pthread_mutex_unlock(&sem_clientes[cliente->cliente_id]);
            pthread_exit(NULL);
        }
        pthread_mutex_lock(&mutex_log);
        writeLogMessage(cliente->nombre, "Cliente cansado, se marcha de la cola");
        pthread_mutex_unlock(&mutex_log);
        iniciarCliente(&clientes[cliente->cliente_id]);
        pthread_mutex_unlock(&sem_clientes[cliente->cliente_id]);
        pthread_exit(NULL);
    }
}


void *caja_1(void *prc){
    if(clientesCaja1==10){
        sleep(20);
        clientesCaja1 = 0;
    }
    pthread_mutex_lock(&mutex_1);
    char msg[100];
    Cliente* cliente = (Cliente*)prc;
    if(cliente->problema_precio==1){
        //Llamamos al repo
        strcpy(msg, "Ha habido un problema con el precio y se ha llamado al reponedor, en la caja 1");
        pthread_mutex_lock(&mutex_repo);
        pthread_cond_signal(&cond_repo);
        pthread_mutex_unlock(&mutex_repo);
    }
    if(cliente->problema_pagar){
        //no atendemos al cliente
        writeLogMessage(cliente->nombre, "No se ha atendido al cliente en la caja 1");
        pthread_mutex_unlock(&mutex_1);
        pthread_exit(NULL);
    }
    ingresosCaja1 = ingresosCaja1 + cliente->precio_compra;
    sleep(cliente->tiempo_caja);
    writeLogMessage(cliente->nombre, strcat(msg,"Ha sido atendido en la caja 1"));
    pthread_mutex_unlock(&mutex_1);
}

void *caja_2(void *prc){
    if(clientesCaja2==10){
        sleep(20);
        clientesCaja2 = 0;
    }
    pthread_mutex_lock(&mutex_2);
    char msg[100];
    Cliente* cliente = (Cliente*)prc;
    if(cliente->problema_precio==1){
        //Llamamos al repo
        strcpy(msg, "Ha habido un problema con el precio y se ha llamado al reponedor, en la caja 2");
        pthread_mutex_lock(&mutex_repo);
        pthread_cond_signal(&cond_repo);
        pthread_mutex_unlock(&mutex_repo);
    }
    if(cliente->problema_pagar){
        //no atendemos al cliente
        writeLogMessage(cliente->nombre, "No se ha atendido al cliente en la caja 2");
        pthread_mutex_unlock(&mutex_2);
        pthread_exit(NULL);
    }
    ingresosCaja2 = ingresosCaja2 + cliente->precio_compra;
    sleep(cliente->tiempo_caja);
    writeLogMessage(cliente->nombre,strcat(msg,"Ha sido atendido en la caja 2"));
    pthread_mutex_unlock(&mutex_2);
}

void *caja_3(void *prc){
    if(clientesCaja3==10){
        sleep(20);
        clientesCaja3 = 0;
    }
    pthread_mutex_lock(&mutex_3);
    char msg[100];
    Cliente* cliente = (Cliente*)prc;
    if(cliente->problema_precio==1){
        //Llamamos al repo
        strcpy(msg, "Ha habido un problema con el precio y se ha llamado al reponedor, en la caja 3");
        pthread_mutex_lock(&mutex_repo);
        pthread_cond_signal(&cond_repo);
        pthread_mutex_unlock(&mutex_repo);
    }
    if(cliente->problema_pagar){
        //no atendemos al cliente
        writeLogMessage(cliente->nombre, "No se ha atendido al cliente en la caja 3");
        pthread_mutex_unlock(&mutex_3);
        pthread_exit(NULL);
    }
    ingresosCaja2 = ingresosCaja2 + cliente->precio_compra;
    sleep(cliente->tiempo_caja);
    writeLogMessage(cliente->nombre,strcat(msg,"Ha sido atendido en la caja 3"));
    pthread_mutex_unlock(&mutex_3);
}

void *reponer(void *prc){
    sleep((rand() % 6) + 5); 
    pthread_exit(NULL);
}

void writeLogMessage(char *id, char *msg) {
    // Calculamos la hora actual
    time_t now = time(0);
    struct tm *tlocal = localtime(&now);
    char stnow[25];
    strftime(stnow, 25, "%d/%m/%y %H:%M:%S", tlocal);
    
    // Escribimos en el log
    FILE *logFile = fopen(logFileName, "a");
    fprintf(logFile, "[%s] %s: %s\n", stnow, id, msg);
    fclose(logFile);
}



int main() {
    pthread_t caja1, caja2, caja3, reponedor;
    for (int i = 0; i < 20; i++) {
        pthread_mutex_init(&sem_clientes[i], NULL);
    }
    for (int i = 0; i < 20; i++) {
        iniciarCliente(&clientes[i]);
    }
    pthread_mutex_init(&mutex_1, NULL);
    pthread_mutex_init(&mutex_2, NULL);
    pthread_mutex_init(&mutex_3, NULL);
    pthread_mutex_init(&mutex_repo, NULL);
    pthread_mutex_init(&mutex_log, NULL);
    pthread_cond_init(&cond_repo, NULL);

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    sigaction(SIGUSR1, &sa, NULL);

    memset(&sa_ignore, 0, sizeof(sa_ignore));
    sa_ignore.sa_handler = SIG_IGN;


    int cliente_1 = -1;
    int cliente_2 = -1;
    int cliente_3 = -1;

    int errorCaja1crate = 0;
    int errorCaja2crate = 0;
    int errorCaja3crate = 0;
    int errorCaja1join = 0;
    int errorCaja2join = 0;
    int errorCaja3join = 0;

    while(1){
        cliente_1 = cliente_libre();
        if(cliente_1!=-1){
            pthread_mutex_lock(&sem_clientes[cliente_1]);
            errorCaja1crate = pthread_create(&caja1, NULL, caja_1, &clientes[cliente_1]);
            if(errorCaja1crate!=0){
                printf("Error al crear el hilo de la caja 1\n");
            }
            errorCaja1join = pthread_join(caja1, NULL);
            if(errorCaja1join!=0){
                printf("Error al hacer join del hilo de la caja 1\n");
            }
            iniciarCliente(&clientes[cliente_1]);
            cliente_1=-1;
            pthread_mutex_unlock(&sem_clientes[cliente_1]);
        }
        cliente_2 = cliente_libre();
        if(cliente_2!=-1){
            pthread_mutex_lock(&sem_clientes[cliente_2]);
            errorCaja2crate = pthread_create(&caja2, NULL, caja_2, &clientes[cliente_2]);
            if(errorCaja2crate!=0){
                printf("Error al crear el hilo de la caja 2\n");
            }
            errorCaja2join = pthread_join(caja2, NULL);
            if(errorCaja2join!=0){
                printf("Error al hacer join del hilo de la caja 2\n");
            }
            iniciarCliente(&clientes[cliente_2]);
            cliente_2=-1;
            pthread_mutex_unlock(&sem_clientes[cliente_2]);
        }
        cliente_3 = cliente_libre();
        if(cliente_3!=-1){
            pthread_mutex_lock(&sem_clientes[cliente_3]);
            errorCaja3crate = pthread_create(&caja3, NULL, caja_3, &clientes[cliente_3]);
            if(errorCaja3crate!=0){
                printf("Error al crear el hilo de la caja 3\n");
            }
            errorCaja3join = pthread_join(caja3, NULL);
            if(errorCaja3join!=0){
                printf("Error al hacer join del hilo de la caja 3\n");
            }
            iniciarCliente(&clientes[cliente_3]);
            cliente_3=-1;
            pthread_mutex_unlock(&sem_clientes[cliente_3]);
        }
    }


    
}