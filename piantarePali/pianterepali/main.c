#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

typedef struct
{
    pthread_mutex_t palo1;
    pthread_mutex_t palo2;

} Semafori;

Semafori *semafori;

void checkError(int response, char *errorMessage)
{
    if (response)
    {
        printf("%s", errorMessage);
        exit(-1);
    }
}

void *martellatore1(void *args)
{
    printf("dio can");

    pthread_exit(NULL);
}

void *martellatore2(void *args)
{
    printf("dio can");
    pthread_exit(NULL);
}

void *tienePalo(void *args)
{
    int response;
    while (1)
    {
        response = pthread_mutex_lock(&semafori->palo1);
        checkError(response, "errore lock");
        response = pthread_mutex_lock(&semafori->palo2);
        checkError(response, "errore lock");

        printf("pali Presi!");

        sleep(10);

        printf("martellate schiavi!");

        response = pthread_mutex_unlock(&semafori->palo1);
        checkError(response, "errore unlock");

        response = pthread_mutex_unlock(&semafori->palo2);
        checkError(response, "errore unlock");
    }
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    int response, i;
    pthread_mutexattr_t mattr;
    pthread_t threads[3];
    semafori = (Semafori *)malloc(sizeof(Semafori));

    response = pthread_mutexattr_init(&mattr);
    checkError(response, "mutexAttributeError");

    response = pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
    checkError(response, "porcodio");

    response = pthread_mutex_init(&semafori->palo1, &mattr);
    checkError(response, "error mutexInit");
    response = pthread_mutex_init(&semafori->palo2, &mattr);
    checkError(response, "error mutexInit");

    response = pthread_create(&threads[0], NULL, tienePalo, NULL);
    checkError(response, "error creating thread for tiene palo");

    for (i = 1; i < 3; i++)
    {
        if (i == 1)
        {
            response = pthread_create(&threads[i], NULL, martellatore1, NULL);
        }
        else
        {
            response = pthread_create(&threads[i], NULL, martellatore2, NULL);
        }
        checkError(response, "error creating thread for martellatore" + i);
    }

    for (i = 0; i < 3; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
