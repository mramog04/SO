#include <unistd.h>
/* The following example executes the ls command , specifying the pathname of the executable (/ bin / ls
) and using arguments supplied directly to the command to produce single - column output . */

/* El siguiente ejemplo ejecuta el comando ls, especificando la ruta del ejecutable (/bin/ls) 
y utilizando argumentos suministrados directamente al comando para producir una salida en una sola columna.
 */int ret ;
ret = execl ( " / bin / ls " , " ls " , " -l " , ( char *) 0) ;

/* The following example is similar to Using execl () . In addition , it specifies the environment for
the new process image using the env argument . */

/* El siguiente ejemplo es similar al uso de execl(). Además, especifica el entorno para la nueva imagen de proceso utilizando el argumento env.
 */int ret ;
char * env [] = { " HOME =/ usr / home " , " LOGNAME = home " , ( char *) 0 };
ret = execle ( " / bin / ls " , " ls " , " -l " , ( char *) 0 , env ) ;

/* The following example searches for the location of the ls command among the directories specified
by the PATH environment variable . */

/* El siguiente ejemplo busca la ubicación del comando ls entre los directorios especificados por la variable de entorno PATH.*/

int ret ;
ret = execlp ( " ls " , " ls " , " -l " , ( char *) 0) ;

/* The following example passes arguments to the ls command in the cmd array . */

/* El siguiente ejemplo pasa argumentos al comando ls mediante el arreglo cmd.*/
char * cmd [] = { " ls " , " -l " , ( char *) 0 };
int ret ;
ret = execv ( " / bin / ls " , cmd ) ;

/* The following example passes arguments to the ls command in the cmd array , and specifies the
environment for the new process image using the env argument . */

/*El siguiente ejemplo pasa argumentos al comando ls mediante el arreglo cmd 
y especifica el entorno para la nueva imagen de proceso utilizando el argumento env */
int ret ;
char * cmd [] = { " ls " , " -l " , ( char *) 0 };
char * env [] = { " HOME =/ usr / home " , " LOGNAME = home " , ( char *) 0 };
ret = execve ( " / bin / ls " , cmd , env ) ;

/* The following example searches for the location of the ls command among the directories specified
by the PATH environment variable , and passes arguments to the ls command in the cmd array . */

/* El siguiente ejemplo busca la ubicación del comando ls entre los directorios especificados por la variable de entorno PATH
 y pasa argumentos al comando ls mediante el arreglo cmd. */
int ret ;
char * cmd [] = { " ls " , " -l " , ( char *) 0 };
ret = execvp ( " ls " , cmd ) ;