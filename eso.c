#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_TOKENS 100 // Maximum number of tokens to parse
#define MAX_TOKEN_LENGTH 100 // Maximum length of each token

volatile sig_atomic_t sigint_received = 0;

void handler(int sig) {
    sigint_received = 1;
}

void tokenize(char *str, char **tokens, int *num_tokens) {
    *num_tokens = 0;
    char *tok = strtok(str, " ");
    while (tok != NULL && *num_tokens < MAX_TOKENS) {
        int len = strlen(tok);
        if (len > MAX_TOKEN_LENGTH - 1) {
            printf("Error: token '%s' is too long.\n", tok);
            return;
        }
        for (int i = 0; i < len; i++) {
            tok[i] = tolower(tok[i]); // convert each token to lowercase
        }
        tokens[*num_tokens] = tok;
        (*num_tokens)++;
        tok = strtok(NULL, " ");
    }
    tokens[*num_tokens] = NULL;
}

int main() {
    char str[1024];
    char *tokens[MAX_TOKENS];
    int num_tokens;

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa)); // Inicializa la estructura a cero
    sa.sa_handler = handler;
    sigaction(SIGINT, &sa, NULL);

    while (1) {
        if (sigint_received) {
            char op[2];
            while (1) {
                printf("\nSIGINT received\n");
                printf("Quieres salir del programa? (s/n): ");
                if (fgets(op, sizeof(op), stdin) != NULL) {
                    // Consumir el carácter de nueva línea que queda en el búfer de entrada
                    while (getchar() != '\n');
                    char opcion = op[0]; // Obtener solo el primer carácter
                    if (opcion == 's') {
                        printf("Programa finalizado.\n");
                        return 0;
                    } else if (opcion == 'n') {
                        printf("Continuando...\n");
                        sigint_received = 0; // Resetear la bandera
                        break;
                    } else {
                        printf("Opcion no valida\n");
                    }
                }
            }
        }

        printf("Introduce un comando (p.e. ls -l -a): ");
        fflush(stdout);
        if (fgets(str, sizeof(str), stdin) == NULL) {
            perror("Error al leer la entrada");
            continue;
        }
        str[strlen(str) - 1] = '\0'; // Eliminar el salto de línea
        tokenize(str, tokens, &num_tokens);

        int pid = fork();
        if (pid == 0) {
            // Proceso hijo
            if (execvp(tokens[0], tokens) == -1) {
                printf("Error al ejecutar el comando '%s': %s\n", tokens[0], strerror(errno));
            }
            return 1; // Salir del proceso hijo si execvp falla
        } else {
            // Proceso padre
            wait(NULL);
        }
    }

    printf("Programa finalizado.\n");
    return 0;
}