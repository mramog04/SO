#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
void nonada(int s)
{printf("PADRE: Estoy en la manejadora.\n");
}
int main(void)
{char lInea[40];
int valor_devuelto;
struct sigaction ss;
switch (fork())
{case -1: perror("fork"); return 1;
case 0: /* HIJO */
for (;;) pause();
default: /* PADRE */
ss.sa_handler=nonada;
if (-1==sigaction(SIGUSR1,&ss,NULL))
{perror("PADRE: sigaction");
return 1;}
printf("PADRE: SIGUSR1(%d) registrada.\n",SIGUSR1);
if (wait(&valor_devuelto)==-1)
{perror("PADRE: wait");
return 1;}
}
return 0;
}