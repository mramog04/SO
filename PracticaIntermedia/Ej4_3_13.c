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
        int pid2 = fork();
        if(pid2==0){
            //soy el hijo del padre
            int pidabuelo2;
            close(fildes[1]);
            read(fildes[0],&pidabuelo2,sizeof(pidabuelo2));
            close(fildes[0]);
            printf("[H] Mi padre=%d, yo=%d, mi abuelo=%d\n",getppid(),getpid(),pidabuelo2);
        }else{
            close(fildes[0]);
            int pidabuelo = getppid();
            write(fildes[1],&pidabuelo,sizeof(pidabuelo));
            close(fildes[1]);
            printf("[P] Mi padre=%d, yo=%d, mi hijo=%d\n",getppid(),getpid(),pid2);
            wait(NULL);
        }
    }else{
        printf("Soy el abuelo\n");
        wait(NULL);
    }
    return 0;
}