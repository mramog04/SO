#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>


int main() {
    // Tu código aquí
    int fildes[2];
    pipe(fildes);
    char buffer[1024];
    int pid = fork();
    if(pid == 0){
        close(fildes[0]);
        write(fildes[1],"Hola mundo",11);
        fildes[sizeof(fildes)-1] = '\0';
        close(fildes[1]);
    }else{
        close(fildes[1]);
        read(fildes[0],buffer,1024);
        printf("El contenido del buffer es: %s\n",buffer);
        close(fildes[0]);
    }
    return 0;
}