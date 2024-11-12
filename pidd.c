#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main (void) {
pid_t pid;
int estado=10;
pid=fork();
if(pid!=0){
    pid=wait(NULL);
    printf("Pid de proceso abuelo: %d",pid);
}else if(pid==0){
    pid=fork();
    if(pid!=0){
        pid=wait(&estado);
        printf("Pid de proceso padre: %d",pid); 
    }else{
        pid=fork();
        pid=wait(NULL);
        printf("Pid de proceso hijo: %d",pid);
    }
}

}