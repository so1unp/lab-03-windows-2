#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>    // para señales
#include <sys/times.h> // times()

/*El programa forkprio.c debe crear un cierto
número de procesos hijos, cada uno ejecutando con un valor de prioridad
progresivamente más bajo. Luego de un cierto número de segundos se debe envíar
 una señal de terminación (SIGTERM) a todos los procesos hijos. Estos, antes de
  terminar su ejecución, deben imprimir su PID, prioridad y el número de segundos
   que ejecutaron.*/

int busywork(void)
{
    struct tms buf;
    for (;;)
    {
        times(&buf);
    }
}

void handler(int signum)
{

    // imprime el número de la señal y su descripcion
    printf("Child %d (nice %2d):\t%3li\n", getpid(), 0);
}

int main(int argc, char *argv[])
{

    int cantHijos = atoi(argv[1]);
    int seg = atoi(argv[2]);
    // int reducirPrioridad = atoi(argv[3]);

    // estructura para manejar señales
    struct sigaction sa;
    sa.sa_handler = handler; // funcion que maneja la señal
    sa.sa_flags = 0;

    sigaction(15, &sa, NULL);

    __pid_t pidHijos[cantHijos];

    __pid_t pidValue = 0;

    for (int i = 0; i < cantHijos; i++)
    {

        pidValue = fork();
        if (pidValue == 0)
        {
            printf("%d : %d\n", getpid(), i);
            pidHijos[i] = getpid();
            busywork();
        }
    }

    sleep(seg);
    __pid_t pgid = getpgid(pidValue);
    kill(-pgid, SIGTERM);

    exit(EXIT_SUCCESS);
}
