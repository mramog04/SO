#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
//Este codigo no esta lo mejor hecho pero mas o menos funciona
int main(int argc, char *argv[]){
    int path1[1024];
    int path2[1024];
    char buffer[4096];
    int desc1, desc2;
    if(argc == 1){
        printf("No se ha ingresado ninguna ruta a un archivo\n");
    }else if(argc == 2){
        printf("Son necearios dos directorios\n");
    }else{
        strncpy(path1,argv[1],sizeof(path1)-1);
        strncpy(path2,argv[2],sizeof(path2)-1);
        path1[sizeof(path1)-1] = '\0';
        path2[sizeof(path2)-1] = '\0';
        if(access(path1,F_OK)==-1){
            printf("Error en los path\n");
            exit(0);
        }
        if(access(path2,F_OK)==-1){
            printf("Error en los path\n");
            exit(0);
        }
        desc1 = open(path1,O_RDONLY);
        desc2 = open(path2,O_TRUNC | O_WRONLY);
        read(desc1,buffer,4096);
        buffer[sizeof(buffer)-1] = '\0';
        write(desc2,buffer,strlen(buffer));
        close(desc1);
        close(desc2);
        //printf("Se ha copiado el contenido de %s a %s\n",path1,path2);
        printf("El contenido del archivo es: %s\n",buffer);
        exit(0);
    }
    return 0;
}