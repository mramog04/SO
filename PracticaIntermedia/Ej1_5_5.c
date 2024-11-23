#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    printf("Este es mi pid %d\n",getpid());
    int ret;
    ret = execlp ( "cal" , "cal" ,(char*) 0);
    printf("La fecha de hoy es: %d\n",ret);
    printf("Este es mi pid %d\n",getpid());
    return 0;
}