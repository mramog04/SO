#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void nonada(int s) {
    printf("PADRE: Estoy en la manejadora.\n");
}

int main() {
    pid_t pidHijo, pidNieto, pidVisnieto, pidTataranieto;
    struct sigaction ss;

    // Configurar la acción para ignorar SIGINT
    ss.sa_handler = SIG_IGN;


    sigaction(SIGINT, &ss, NULL); // Ignorar SIGINT en el proceso padre

    // Crear procesos
    pidHijo = fork();

    if (pidHijo < 0) {
        perror("Error al obtener el pid del hijo\n");
        exit(0);
    } else if (pidHijo == 0) {
        // En el proceso hijo también se ignora SIGINT
        sigaction(SIGINT, &ss, NULL);
        execlp("xeyes", "xeyes", (char *)NULL);
    } else {
        pidNieto = fork();
        if (pidNieto < 0) {
            perror("Error al obtener el ppid del nieto\n");
        } else if (pidNieto == 0) {
            // En el nieto también ignoramos SIGINT
            sigaction(SIGINT, &ss, NULL);
            execlp("xlogo", "xlogo", (char *)NULL);
        } else {
            pidVisnieto = fork();
            if (pidVisnieto < 0) {
                perror("Error");
            } else if (pidVisnieto == 0) {
                // En el visnieto ignoramos SIGINT
                sigaction(SIGINT, &ss, NULL);
                execlp("xcalc", "xcalc", (char *)NULL);
            } else {
                pidTataranieto = fork();
                if (pidTataranieto < 0) {
                    perror("Error 2");
                } else if (pidTataranieto == 0) {
                    // En el tataranieto ignoramos SIGINT
                    sigaction(SIGINT, &ss, NULL);
                    execlp("xclock", "xclock", "-update", "1", (char *)NULL);
                } else {
                    sigaction(SIGINT, &ss, NULL);
                    wait(NULL);  // Espera al tataranieto
                }
            }
            sigaction(SIGINT, &ss, NULL);
            wait(NULL);  // Espera al visnieto
        }
        sigaction(SIGINT, &ss, NULL);
        wait(NULL);  // Espera al nieto
    }
    wait(NULL);  // Espera al hijo
    sigaction(SIGINT, &ss, NULL);
    // Ejecutar el último comando en el proceso padre
    execlp("xload", "xload", (char *)NULL);

    return 0;
}
