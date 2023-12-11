#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define NAlfa 5
#define NBeta 4

#define TURNODIALFA 1
#define TURNODIBETA 0

typedef struct
{
    pthread_mutex_t pista;
    int nCorridoriAlfaWented;
    int nCorridoriBetaWented;
    pthread_cond_t waitingCondAlfa;
    pthread_cond_t waitingCondBeta;
    int turno;
} Semaphores;

Semaphores *semafori;

void checkError(int response, char *errorMessage)
{
    if (response)
    {
        printf("%s", errorMessage);
        exit(-1);
    }
}

void *corridoreAlfa(void *args)
{
    int response;
    int idCorridore = *(int *)args;
    int giriEffettuati = 0;

    while (1)
    {
        response = pthread_mutex_lock(&semafori->pista);
        checkError(response, "lock error alfa");

        while (semafori->turno != TURNODIALFA || semafori->nCorridoriAlfaWented != idCorridore)
        {
            response = pthread_cond_wait(&semafori->waitingCondAlfa, &semafori->pista);
            checkError(response, "cond wait error");
        }
        printf("il corridore A%i inizia il giro N %d!\n", idCorridore, giriEffettuati);
        response = pthread_mutex_unlock(&semafori->pista);
        checkError(response, "unlock alfa error");

        sleep(1);

        response = pthread_mutex_lock(&semafori->pista);
        checkError(response, "mutexLockBETA");

        printf("il corridore A%i finisce il giro N %d!\n", idCorridore, giriEffettuati);
        giriEffettuati++;
        if (semafori->nCorridoriAlfaWented == NAlfa - 1)
        {
            semafori->nCorridoriAlfaWented = 0;
        }
        else
        {
            semafori->nCorridoriAlfaWented++;
        }
        semafori->turno = TURNODIBETA;

        response = pthread_cond_broadcast(&semafori->waitingCondAlfa);
        checkError(response, "broadcast error ");
        response = pthread_mutex_unlock(&semafori->pista);
        checkError(response, "unlock ALFA ERROR");
    }
    pthread_exit(NULL);
}

void *corridoreBeta(void *args)
{
    int response;
    int idCorridore = *(int *)args;

    while (1)
    {

        response = pthread_mutex_lock(&semafori->pista);
        checkError(response, "mutexLockBETA");
        while (semafori->turno != TURNODIBETA || semafori->nCorridoriBetaWented != idCorridore)
        {
            response = pthread_cond_wait(&semafori->waitingCondBeta, &semafori->pista);
            checkError(response, "condWAIT ERror");
        }
        printf("il corridore B%i inizia il giro!\n", idCorridore);
        response = pthread_mutex_unlock(&semafori->pista);
        checkError(response, "unlock error");

        sleep(1);

        response = pthread_mutex_lock(&semafori->pista);
        checkError(response, "locl error");

        printf("il corridore B%i finisce il giro !\n", idCorridore);

        if (semafori->nCorridoriBetaWented == NBeta - 1)
        {
            semafori->nCorridoriBetaWented = 0;
        }
        else
        {
            semafori->nCorridoriBetaWented++;
        }
        semafori->turno = TURNODIALFA;

        response = pthread_cond_broadcast(&semafori->waitingCondBeta);
        checkError(response, "broadCast ERRO");
        response = pthread_mutex_unlock(&semafori->pista);
        checkError(response, "unlock error");
    }
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    pthread_t ids[NAlfa+NBeta];
    int response;
    int i;
    pthread_mutexattr_t mattr;
    pthread_condattr_t cvattr;

    semafori = (Semaphores *)malloc(sizeof(Semaphores));
    semafori->nCorridoriAlfaWented = 0;
    semafori->nCorridoriBetaWented = 0;
    semafori->turno = TURNODIALFA;

    response = pthread_mutexattr_init(&mattr);
    checkError(response, "mutexAttributeError");

    response = pthread_mutexattr_setpshared(&mattr,PTHREAD_PROCESS_SHARED);
    checkError(response,"porcodio");

    response = pthread_condattr_init(&cvattr);
    checkError(response, "condAttributeError");

    response=pthread_condattr_setpshared(&cvattr,PTHREAD_PROCESS_SHARED);
    checkError(response,"porcodioPARTE2");

    response = pthread_mutex_init(&semafori->pista, &mattr);
    checkError(response, "mutex ERROR");

    response = pthread_cond_init(&semafori->waitingCondAlfa, &cvattr);
    checkError(response, "condInitError");

    response = pthread_cond_init(&semafori->waitingCondBeta, &cvattr);
    checkError(response, "condInitError");

    for (i = 0; i < NAlfa; i++)
    {
        pthread_create(&ids[i],NULL,corridoreAlfa,&i);
    }

    for (i = 0; i < NBeta; i++)
    {
        pthread_create(&ids[i+NAlfa],NULL,corridoreBeta,&i);
    }

    for (i = 0; i < NBeta+NAlfa; i++)
    {
        pthread_join(ids[i],NULL);
    }
    

    return 0;
}
