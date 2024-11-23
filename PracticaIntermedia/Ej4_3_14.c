#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>


int main() {
    int fildes[2];
    pipe(fildes);
    int pid = fork();
    if(pid == 0){
        close(fildes[0]);
        dup2(fildes[1],STDOUT_FILENO);
        close(fildes[1]);
        execlp("ls","ls","-l",(char *)0);
        exit(0);
    }else{
        
        close(fildes[1]);
        dup2(fildes[0],STDIN_FILENO);
        close(fildes[0]);
        execlp("wc","wc","-l",(char *)0);
        close(fildes[0]);
    }
    return 0;
}