#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    pid_t pidHijo,pidNieto,pidVisnieto;

    pidHijo = fork();

    if(pidHijo<0){
        perror("Error al obtener el pid del hijo\n");
        exit(0);
    }else if(pidHijo==0){
        printf("Soy el proceso hijo con el pidd %d\n",getpid());

        pidNieto=fork();
        if(pidNieto<0){
            perror("Error al obtener el ppid del nieto\n");
        }else if(pidNieto==0){
            printf("Soy el nieto y mi pid es:%d\n",getpid());

            pidVisnieto=fork();
            if(pidVisnieto<0){
                perror("EEROR");
            }else if(pidVisnieto==0){
                printf("Soy el visnieto %d,(mi pid)",getpid());
            }else{
                wait(NULL);
            }
        }else{
            wait(NULL);
        }
    
    }else{
        printf("Soy el proceso padre, este es mi pid:%d, y este es el de min hijo:%d\n",getpid(),pidHijo);
        wait(NULL);
    }
    return(0);
}