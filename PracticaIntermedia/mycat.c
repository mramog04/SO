#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc,char *argv[]){
    char path[1024];
    int desc;
    char mensj[4096];
    if(argc== 1){
        printf("No such file or directory\n");
        exit(0);
    }else{
        strncpy(path,argv[1],sizeof(path)-1);
        path[sizeof(path)-1] = '\0';
        if(access(path,F_OK)==-1){
            printf("No such file or directory\n");
            exit(0);
        }else{
            
            desc = open(path,O_RDWR);
            read(desc,mensj,1024);
            mensj[sizeof(mensj)-1] = '\0';
            printf("El contenido del archivo es:\n %s\n",mensj);
            close(desc);
            exit(0);
        }
    }
}