#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main ( void ) {
    int pid = fork () ;
    if(pid ==0){
        sleep(1);
        printf ( " [ H ] ppid = %5d , pid = %5d \n " , getppid () , getpid () );
        exit(33);
        }else{
        int status,p;
        printf ( " [ P ] ppid = %5d , pid = %5d , H = %5d \n " , getppid () , getpid () , pid ) ;
        p = waitpid(pid,&status,0);
        printf("[P] el proceso pid=%d  acaba de terminar con estado %d\n",p,status);
    return 0;
    }
}