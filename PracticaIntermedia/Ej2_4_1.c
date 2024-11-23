#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

struct sigaction sa;
void handler (int sig) {
    printf ("SIGINT received %d\n", sig);
    exit(0);
}
int main(void) {
    memset(&sa, 0, sizeof(sa)); // Inicializa la estructura a cero
    sa.sa_handler = SIG_IGN;
    sigaction (SIGINT , &sa , NULL);
    while (1) {

    }
    return 0;
}