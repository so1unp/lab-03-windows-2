#define _POSIX_C_SOURCE 200809L
#include <stdio.h>     // para printf()
#include <stdlib.h>    // para exit()
#include <signal.h>    // para señales
#include <string.h>    // para strsignal()
#include <unistd.h>    // para pause()


// funcion que se ejecuta cuando llega una señal
void handler(int signum) {

    // imprime el número de la señal y su descripcion
    printf("%d: %s\n", signum, strsignal(signum));

    fflush(stdout); 
}

int main(int argc, char *argv[]) {

    
    // estructura para manejar señales
    struct sigaction sa;
    sa.sa_handler = handler; // funcion que maneja la señal    
    sa.sa_flags = 0;                
    sigemptyset(&sa.sa_mask); // esto es para no bloquear ninguna señal mientras se ejecute el proceso

         

    // registra el manejador handler() para todas las señales 
    for (int i = 1; i < _NSIG; i++) {        
        sigaction(i, &sa, NULL);    
    }

   // el proceso se suspende hasta que llegue una señal
    while (1) {
        pause();
    }

    return EXIT_SUCCESS;
}


/*
La señales que no se pueden manejar son:
SIGKILL y SIGSTOP. Estas señales no pueden ser capturadas, ignoradas o bloqueadas.
SIGKILL: Se utiliza para terminar un proceso de forma inmediata.
SIGSTOP: Se utiliza para detener (suspender) un proceso.
*/