#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>

#define MAX_TOKENS 100 // Maximum number of tokens to parse
#define MAX_TOKEN_LENGTH 100 // Maximum length of each token

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
    printf("Introduce un commando (p.e. ls -l -a): ");
    fflush(stdout);
    fgets(str, sizeof(str), stdin);
    str[strlen(str) - 1] = '\0';
    tokenize(str, tokens, &num_tokens);
    printf("Number of tokens: %d\n", num_tokens);
    for (int i = 0; i < num_tokens; i++) {
        printf("Token %d: %s\n", i + 1, tokens[i]);
    }
    if (execvp(tokens[0], tokens) == -1)
        printf("Error al ejecutar el comando '%s': %s\n", tokens[0], strerror(errno));
    return 0;
}