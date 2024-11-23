#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc,char *argv[]){
    char path[1024];
    int desc;
    char *mensj = "Hola mundo";
    if(argc== 1){
        getcwd(path,1024);
        strcat(path,"/archivo.txt");
        desc = open(path,O_CREAT | O_RDWR,0644);
        write(desc,mensj,strlen(mensj));
        close(desc);
        exit(0);
    }else{
        //usar strncopy
        strncpy(path,argv[1],sizeof(path)-1);
        path[sizeof(path)-1] = '\0';//no entiendo del todo para lo que sirve esta linea
        if(access(path,F_OK)==-1){
            getcwd(path,1024);
            strcat(path,"/archivo.txt");
            desc = open(path,O_CREAT,111);
            write(desc,mensj,strlen(mensj));
            close(desc);
            exit(0);
        }else{
            desc = open(path,O_TRUNC | O_RDWR);
            write(desc,mensj,strlen(mensj));
            close(desc);
            exit(0);
        }
    }
    return 0;   
}