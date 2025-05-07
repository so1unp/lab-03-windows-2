#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/times.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

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

void handler()
{

    struct rusage usage;                                       // estructura para almacenar el uso de recursos
    getrusage(RUSAGE_SELF, &usage);                            // obtiene info sobre el uso de recursos del proceso actual (RUSAGE_SELF)
    int prio = getpriority(PRIO_PROCESS, 0);                   // obtiene la prioridad del proceso actual (PRIO_PROCESS para un proceso en especifico)
    long secs = usage.ru_stime.tv_sec + usage.ru_utime.tv_sec; // obtiene el tiempo de CPU utilizado en segundos
    printf("Child %d (nice %2d):\t%3ld\n", getpid(), prio, secs);
    exit(0);
}

int main(int argc, char *argv[])
{

    int cantHijos = atoi(argv[1]);
    int seg = atoi(argv[2]);
    int reducirPrioridad = atoi(argv[3]);

    // estructura para manejar señales
    struct sigaction sa;
    sa.sa_handler = handler; // funcion que maneja la señal
    sa.sa_flags = 0;

    sigaction(SIGTERM, &sa, NULL); // configura el handler para cuando llegue la señal SIGTERM

    __pid_t pidValue = 0;

    // padre se convierte en lider de grupo
    setpgid(0, 0);

    for (int i = 0; i < cantHijos; i++)
    {

        pidValue = fork();
        if (pidValue == 0)
        {
            // cada hijo se une al grupo del padre
            setpgid(0, getppid());
            if (reducirPrioridad)
                nice(i); // incrementa nice -> reduce prioridad

            printf("%d : %d\n", getpid(), i);
            // pidHijos[i] = getpid();
            busywork();
        }
    }

    if (seg > 0)
    {
        sleep(seg);
        // enviar SIGTERM a todo el grupo del padre (todos los hijos)
        kill(0, SIGTERM);
        exit(EXIT_SUCCESS);
    }
    else
    {
        // ejecutar indefinidamente
        pause();
    }
}
