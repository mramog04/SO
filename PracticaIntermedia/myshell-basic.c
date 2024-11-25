#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <setjmp.h>



#define MAX_TOKENS 100 // Maximum number of tokens to parse
#define MAX_TOKEN_LENGTH 100 // Maximum length of each token

struct sigaction sa,sa_ignore;

void handler (int sig) {

    /* memset(&sa2, 0, sizeof(sa2)); 
    sa2.sa_handler = SIG_IGN;
    sigaction (SIGINT , &sa2 , NULL); */
    sigaction (SIGINT , &sa_ignore , NULL);
    char op[1024];
    while (1) {
        printf("Quieres salir del programa? (s/n): ");
        scanf("%s", op);          
        if (op[0] == 's') {
            exit(0);
        } else if (op[0] == 'n') {
            break;
        } else {
            printf("Opcion no valida\n");
        }
    }
    sigaction (SIGINT , &sa , NULL);
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

int main(){
    char str[1024];
    char *tokens[MAX_TOKENS];
    int num_tokens;
    int signalError = 0;

    memset(&sa, 0, sizeof(sa)); 
    sa.sa_handler = handler;
    sigaction (SIGINT , &sa , NULL);

    memset(&sa_ignore, 0, sizeof(sa_ignore));
    sa_ignore.sa_handler = SIG_IGN;

    printf("Introduce un commando (p.e. ls -l -a): \n");

    while (1) {
        if(signalError == 1){
            printf("Introduce un commando (p.e. ls -l -a): \n");   
            signalError = 0;
        }
        fflush(stdout);

        if (fgets(str, sizeof(str), stdin) == NULL) {
            signalError = 1;
            continue;
        }
        str[strlen(str) - 1] = '\0';
        tokenize(str, tokens, &num_tokens);
        int pid = fork();
        if (pid == 0) {
            if (execvp(tokens[0], tokens) == -1) {
                printf("Error al ejecutar el comando '%s': %s\n", tokens[0], strerror(errno));
                exit(1);
            }
        } else {
            wait(NULL);
        }

    
    }
    return 0;
}