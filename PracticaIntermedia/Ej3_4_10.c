#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc,char *argv[]){
    char path[1024];
    int desc;
    char mensj[1024];
    if(argc== 1){
        printf("No se ha ingresado ninguna ruta a un archivo\n");
    }else{
        strncpy(path,argv[1],sizeof(path)-1);
        path[sizeof(path)-1] = '\0';//no entiendo del todo para lo que sirve esta linea
        if(access(path,F_OK)==-1){
            printf("El archivo no existe\n");
        }else{
            desc = open(path,O_RDWR);
            read(desc,mensj,1024);
            path[sizeof(path)-1] = '\0';//no entiendo del todo para lo que sirve esta linea
            printf("El contenido del archivo es: %s\n",mensj);
            close(desc);
            exit(0);
        }
    }
    return 0;   
}