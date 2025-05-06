#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void test_fork(int count, int wait);
void test_thread(int count, int wait);

int main(int argc, char *argv[])
{
    int modo, wait, count;

    // Chequea los  parametros
    if (argc < 3)
    {
        fprintf(stderr, "Uso: %s [-p | -h] [-w] cantidad\n", argv[0]);
        fprintf(stderr, "\t-p           Crear procesos.\n");
        fprintf(stderr, "\t-t           Crear hilos.\n");
        fprintf(stderr, "\t-w           Esperar que proceso o hilo finalice.\n");
        fprintf(stderr, "\tcantidad     Número de procesos o hilos a crear.\n");
        exit(EXIT_FAILURE);
    }

    modo = argv[1][1]; // debe ser p o t

    if (argc == 4)
    {
        wait = argv[2][1];
        if (wait != 'w')
        {
            fprintf(stderr, "Error: opción invalida %s.\n", argv[2]);
            exit(EXIT_FAILURE);
        }
        count = atoi(argv[3]);
    }
    else
    {
        wait = 0;
        count = atoi(argv[2]);
    }

    if (count <= 0)
    {
        fprintf(stderr, "Error: el contador debe ser mayor que cero.\n");
        exit(EXIT_FAILURE);
    }

    if (modo != 'p' && modo != 't')
    {
        fprintf(stderr, "Error: opción invalida %s.\nUsar -p (procesos) o -t (hilos)\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if (modo == 'p')
    {
        printf("Probando fork()...\n");
        test_fork(count, wait);
    }
    else if (modo == 't')
    {
        printf("Probando pthread_create()...\n");
        test_thread(count, wait);
    }

    exit(EXIT_SUCCESS);
}

//==========================================================================
// Código para la prueba con fork()
//==========================================================================
void test_fork(int count, int wait)
{

    for (int i = 0; i < count; i++)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0)
        {
            // CÃ³digo que ejecuta el proceso hijo
            // printf("Proceso hijo %d creado\n", i + 1);
            _exit(EXIT_SUCCESS);
        }
        if (wait)
        {
            // Espera a que el proceso hijo termine
            waitpid(pid, NULL, 0);
        }
    }
}

//==========================================================================
// Código para la prueba con pthread_create()
//==========================================================================

void *thread_task(void *arg)
{
    int id = *(int *)arg;
    // printf("Hilo %d creado\n", id);
    pthread_exit(NULL);
}

void test_thread(int count, int wait)
{
    pthread_t threads[count]; // Arreglo estático para almacenar hilos
    int ids[count];           // Arreglo estático para almacenar IDs

    for (int i = 0; i < count; i++)
    {
        ids[i] = i + 1; // Asignar ID al hilo
        if (pthread_create(&threads[i], NULL, thread_task, &ids[i]) != 0)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
        if (wait)
        {
            // Espera a que el hilo termine
            pthread_join(threads[i], NULL);
        }
    }

    if (!wait)
    {
        // Si no se espera en cada iteración, esperar al final
        for (int i = 0; i < count; i++)
        {
            pthread_join(threads[i], NULL);
        }
    }
}

/*

1) La creacion de hilos es menos costosa que la creacion de procesos principalmente porque
los hilos comparten el mismo espacio de memoria y recursos del proceso padre, lo que reduce
la sobrecarga de memoria y tiempo de CPU. En contraste, los procesos tienen su propio espacio de
memoria y requieren más recursos para ser creados y gestionados, tambien implica que cambiar
entre hilos dentro de un proceso es mas rapido que cambiar entre procesos, ya que no es necesario
un cambio completo de contexto

2) El uso de de la opcion -w incrementa el tiempo total de ejecucion porque el programa espera
que a que cada proceso o hilo termine antes de crear el siguiente, lo que implica una sincronizacion
secuencial que afecta el rendimiento



*/