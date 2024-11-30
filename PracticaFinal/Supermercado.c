#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <string.h>



#define N 5.0


pthread_mutex_t mutex_1,mutex_2,mutex_3,mutex_repo,mutex_log;
pthread_cond_t cond_repo;
struct sigaction sa;

int clientesCaja1 = 0;
int clientesCaja2 = 0;
int clientesCaja3 = 0;

int ingresosCaja1 = 0;
int ingresosCaja2 = 0;
int ingresosCaja3 = 0;


char logFile[] = "./log.txt";
char logFileName[] = "log.txt";  

Cliente clientes[20];
int num_clientes = 0;

typedef struct {
    int cliente_id;
    char nombre[50];
    int precio_compra;
    int tiempo_caja;
    int problema_precio;
    int problema_pagar;
} Cliente;

void handler(int sig) {
    if(sig == SIGUSR1) {
        if(clientes_lleno()==0){
            pthread_mutex_lock(&mutex_log);
            Cliente* new_cliente = malloc(sizeof(Cliente));
            new_cliente->cliente_id = num_clientes+1;
            strcpy(new_cliente->nombre, "cliente_%d", new_cliente->cliente_id);
            new_cliente->precio_compra = (rand() % 100) + 1;
            new_cliente->tiempo_caja = (rand() % 5) + 1;
            int x = (rand() % 100)+1;
            int y = (rand() % 100)+1;
            if(x>95){
                new_cliente->problema_precio = 1;
            }else{
                new_cliente->problema_precio = 0;
            }
            if(y>75){
                new_cliente->problema_pagar = 1;
            }else{
                new_cliente->problema_pagar = 0;
            }
            for(int i = 0; i < 20; i++) {
                if(clientes[i] == NULL) {
                clientes[i] = new_cliente;
                num_clientes++;
                break;
                }
            }
            writeLogMessage(new_cliente->nombre, "Nuevo cliente en la cola");
            pthread_mutex_unlock(&mutex_log);
            printf("Nuevo cliente: %s\n", new_cliente->nombre);
        }
    }
}

int clientes_lleno() {
    for(int i = 0; i < 20; i++) {
        if(clientes[i] == NULL) {
            return 0;
        }
    }
    return 1;
}

int cliente_libre(){
    int x = -1;
    for(int i = 0; i < 20; i++) {
        if(clientes[i] != NULL) {
            if(clientes[i]->cliente_id < x){
                x = clientes[i]->cliente_id;
            }
        }
    }
    return x;
}

void *cliente(void *prc){
    Cliente* cliente = (Cliente*)prc;
    sleep(cliente->tiempo_cansado);  
    // El tema de los hilos de cliente no entiendo muy bien como va, por que no creo que debamos de tener 20 hilos uno para cada cliente.
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
        msg = "Ha habido un problema con el precio y se ha llamado al reponedor, ";
        pthread_mutex_lock(&mutex_repo);
        pthread_cond_signal(&cond_repo);
        pthread_mutex_unlock(&mutex_repo);
    }
    if(cliente->problema_pagar){
        //no atendemos al cliente
        
    }
    ingresosCaja1 = ingresosCaja1 + cliente->precio_compra;
    sleep(cliente->tiempo_caja);
    writeLogMessage(cliente->nombre, msg + "Ha sido atendido en la caja 1");
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
        msg = "Ha habido un problema con el precio y se ha llamado al reponedor, ";
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
    writeLogMessage(cliente->nombre, msg + "Ha sido atendido en la caja 2");
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
        msg = "Ha habido un problema con el precio y se ha llamado al reponedor, ";
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
    writeLogMessage(cliente->nombre, msg + "Ha sido atendido en la caja 1");
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
    logFile = fopen(logFileName, "a");
    fprintf(logFile, "[%s] %s: %s\n", stnow, id, msg);
    fclose(logFile);
}



int main() {
    pthread_t caja1, caja2, caja3, reponedor;
    pthread_mutex_init(&mutex_1, NULL);
    pthread_mutex_init(&mutex_2, NULL);
    pthread_mutex_init(&mutex_3, NULL);
    pthread_mutex_init(&mutex_repo, NULL);
    pthread_mutex_init(&mutex_log, NULL);
    pthread_cond_init(&cond_repo, NULL);

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    sigaction(SIGUSR1, &sa, NULL);

    int cliente_1 = -1;
    int cliente_2 = -1;
    int cliente_3 = -1;

    while(1){
        if((cliente_libre()=cliente_1)!=-1  /* Aqui hay que buscar al cliente que mas tiempo lleve en la cola */){
            pthread_create(&caja1, NULL, caja_1, *clientes[cliente_1]);
            clientes[cliente_1] = NULL;
        }
        if((cliente_libre()=cliente_2)!=-1){
            pthread_create(&caja2, NULL, caja_2, *clientes[cliente_2]);
            clientes[cliente_2] = NULL;
        }
        if((cliente_libre()=cliente_3)!=-1){
            pthread_create(&caja3, NULL, caja_3, *clientes[cliente_3]);
            clientes[cliente_3] = NULL;
        }
        sleep(2);
    }


    
}