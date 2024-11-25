#define SIGHUP 1 /* colgar */
#define SIGINT 2 /* interrupción (DEL) */
#define SIGQUIT 3 /* salir (ASCII FS) */
#define SIGILL 4 /* instrucción ilegal */
#define SIGTRAP 5 /* trampa de traza (no se restablece cuando se captura) */
#define SIGABRT 6 /* instrucción IOT */
#define SIGBUS 7 /* error de bus */
#define SIGFPE 8 /* excepción de punto flotante */
#define SIGKILL 9 /* matar (no se puede capturar ni ignorar) */
#define SIGUSR1 10 /* señal definida por el usuario # 1 */
#define SIGSEGV 11 /* violación de segmentación */
#define SIGUSR2 12 /* señal definida por el usuario # 2 */
#define SIGPIPE 13 /* escritura en una tubería sin nadie para leerla */
#define SIGALRM 14 /* reloj de alarma */
#define SIGTERM 15 /* señal de terminación de software desde kill */
#define SIGEMT 16 /* instrucción EMT */
#define SIGCHLD 17 /* proceso hijo terminado o detenido */
#define SIGWINCH 21 /* el tamaño de la ventana ha cambiado */