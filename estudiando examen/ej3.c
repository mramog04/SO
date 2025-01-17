#include <stdio.h>
#include <pthread.h>

int global_var = 0;
void *thread_function(void *arg) {
int local_var = *(int *)arg;
global_var += local_var;
printf("Hilo: global_var = %d, local_var = %d\n", global_var, local_var);
return NULL;
}
int main(){
pthread_t thread1, thread2;
int val1 = 5, val2 = 10;
pthread_create(&thread1, NULL, thread_function, &val1);
pthread_create(&thread2, NULL, thread_function, &val2);
pthread_join(thread1, NULL);
pthread_join(thread2, NULL);
printf("Main: global_var = %d\n", global_var);
return 0;
}