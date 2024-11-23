#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int menu(int num_args);
int llamada_fun(int num,char *directorio);
int fun1(char *directorio);
int fun2(char *directorio);
int fun3(char *directorio);
int fun4(char *directorio);
int fun5(char *directorio);
int fun6(char *directorio);
int fun7(char *directorio);
int fun8(char *directorio);
int fun9(char *directorio);
int fun10();


int main(int argc,char *argv[]){
    char directorio_actual[1024];
    char directorio[1024];
    if(argc==1){
        if(getcwd(directorio_actual, sizeof(directorio_actual))!=NULL){
            printf("Se usara la direccion de este archivo como directorio");
            strcpy(directorio,directorio_actual);
        }else{
            perror("Error obteniendo el directorio actual");
            exit(EXIT_FAILURE);
        }
    }else{
        strcpy(directorio,argv[1]);
    }
    int num_args = argc;
    llamada_fun(menu(num_args),directorio);
    return 0;
}

int menu(int num_args){
    int i = 0;
    int menu_op;
    do{
        printf("1) Numero de archivos\n" 
     "2) Numero de subdirectorios\n"
     "3) Fichero mas grande\n"
     "4) Fichero mas pequeño\n"
     "5) Espacio total ocupado\n"
     "6) Número de ficheros con permiso de lectura para el usuario que ejecuta el scrip\n"
     "7) Número de ficheros con permiso de escritura para el usuario que ejecuta el script\n"
     "8) Número de ficheros con permiso de ejecución para el usuario que ejecuta el scrip\n"
     "9) Ficheros con permiso de ejecucion para todos los usuarios\n"
     "10) Salir\n");
     scanf("%i", &menu_op);
    switch (num_args)
    {
    case 1:
        printf("Se va a usar el directorio actual de referencia.");
        while(menu_op<0 || menu_op>10){
            printf("Has introducido mal la ocion del menu.\n Vuelve a introducirlo: ");
            scanf("%i",&menu_op);
        }
        printf("Has elegido la opcion %d", menu_op);
        i=1;
        return menu_op;
        break;
    case 2:
        while(menu_op<0 || menu_op>10){
            printf("Has introducido mal la ocion del menu.\n Vuelve a introducirlo: ");
            scanf("%i",&menu_op);
        }
        printf("Has elegido la opcion %d", menu_op);
        i=1;
        return menu_op;
        break;
    default:
        printf("DATOS INTRODUCIDOS NO VALIDOS");
        exit(0);
        break;
    }
    }while(i==0);
    printf("Ha habido un error");
    exit(0);
}
int llamada_fun(int num,char *directorio){
    switch (num)
    {
    case 1:
        fun1(directorio);
        break;
    case 2:
        fun2(directorio);
        break;
    case 3:
        fun3(directorio);
        break;
    case 4:
        fun4(directorio);
        break;
    case 5:
        fun5(directorio);
        break;
    case 6:
        fun6(directorio);
        break;
    case 7:
        fun7(directorio);
        break;
    case 8:
        fun8(directorio);
        break;
    case 9:
        fun9(directorio);
    case 10:
        fun10();
        break;
    default:
        printf("Ha sucedido un error saliedo del programa.");
        exit(0);
        break;
    }
}

int fun1(char *directorio){
    char comando[256];

    snprintf(comando, sizeof(comando), "ls -l %s | wc -l", directorio);

    int num_archivos=system(comando);

}
int fun2(char *directorio){
    char comando[256];

    snprintf(comando, sizeof(comando), "ls -l %s | wc -l", directorio);

    int num_archivos=system(comando);

}
int fun3(char *directorio){
    char comando[256];

    snprintf(comando, sizeof(comando), "ls -l %s | wc -l", directorio);

    int num_archivos=system(comando);

}
int fun4(char *directorio){
    char comando[256];

    snprintf(comando, sizeof(comando), "ls -l %s | wc -l", directorio);

    int num_archivos=system(comando);

}
int fun5(char *directorio){
    char comando[256];

    snprintf(comando, sizeof(comando), "ls -l %s | wc -l", directorio);

    int num_archivos=system(comando);

}
int fun6(char *directorio){
    char comando[256];

    snprintf(comando, sizeof(comando), "ls -l %s | wc -l", directorio);

    int num_archivos=system(comando);

}
int fun7(char *directorio){
    char comando[256];

    snprintf(comando, sizeof(comando), "ls -l %s | wc -l", directorio);

    int num_archivos=system(comando);

}
int fun8(char *directorio){
    char comando[256];

    snprintf(comando, sizeof(comando), "ls -l %s | wc -l", directorio);

    int num_archivos=system(comando);

}
int fun9(char *directorio){
    char comando[256];

    snprintf(comando, sizeof(comando), "ls -l %s | wc -l", directorio);

    int num_archivos=system(comando);

}
int fun10(){
    printf("Has salido del programa");
    exit(0);
}

