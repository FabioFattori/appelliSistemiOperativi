#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define MaxPosti 2
#define NPersoneSane 4
#define NPersoneUbriache 10 

typedef struct
{
    int id;
    int type; /* ubriaco (1) oppure sano (0) */
    struct Person *next;
} Person;

Person *head;
int passengersSaliti[NPersoneSane + NPersoneUbriache];
int currentPosInArr;

void initializePassengersSaliti()
{
    int i;
    for (i = 0; i < NPersoneSane + NPersoneUbriache; i++)
    {
        passengersSaliti[i] = -1;
    }
}

Person *getAPersonGivingID(int id)
{
    Person *current = head;
    while (current->id != id)
    {
        current = (Person *)current->next;
    }
    return current;
}

void printAllPersons(Person *start)
{

    Person *currentPerson = start;

    while (currentPerson != NULL)
    {
        printf("person id : %i , type : %i\n", currentPerson->id, currentPerson->type);
        currentPerson = (Person *)currentPerson->next;
    }
}

void *DoATrip(void *args)
{
    Person *passengers = (Person *)args;
    printf("CHIUSURA PORTE CON A BORDO => \n");
    printAllPersons(passengers);
    sleep(1);
    pthread_exit(NULL);
}

void *getBack(void *args)
{
    Person *passengers = (Person *)args;
    sleep(1);
    printf("SIAMO ARRIVATI PORCODIO => \n");
    printAllPersons(passengers);

    pthread_exit(NULL);
}

int checkIfPassegerIsAlreadySalito(int id)
{
    int i;
    for (i = 0; i < NPersoneSane + NPersoneUbriache; i++)
    {
        if (passengersSaliti[i] == id)
        {
            return 1;
        }
    }

    return 0;
}

Person *chooseRandomPassengers()
{
    int r;
    Person *response;
    Person *tmp;
    Person *toRet = (Person *)malloc(sizeof(Person));
    do
    {
        r = rand() % (NPersoneSane + NPersoneUbriache);
    } while (checkIfPassegerIsAlreadySalito(r)!=0);
    passengersSaliti[currentPosInArr] = r;
    currentPosInArr++;
    response = getAPersonGivingID(r);
    toRet->id = response->id;
    toRet->type = response->type;
    toRet->next = NULL;

    if (toRet->type == 0)
    {
        do
        {
            do
            {
                r = rand() % (NPersoneSane + NPersoneUbriache);
            } while (checkIfPassegerIsAlreadySalito(r)!=0);

            passengersSaliti[currentPosInArr] = r;
            currentPosInArr++;
            
            tmp = (Person *)malloc(sizeof(Person));
            response = getAPersonGivingID(r);
            tmp->id = response->id;
            tmp->type = response->type;
            tmp->next = NULL;
            toRet->next = (struct Person *)tmp;
        } while (tmp->type == 1);
    }

    return toRet;
}

void startingSetUp()
{
    int i, j;
    Person *currentPerson;
    Person *tmp;
    currentPosInArr = 0;
    for (i = 0; i < NPersoneSane; i++)
    {
        if (head == NULL)
        {
            head = (Person *)malloc(sizeof(Person));
            head->id = 0;
            head->next = NULL;
            head->type = 0;
        }
        else if (head->next == NULL)
        {
            currentPerson = (Person *)malloc(sizeof(Person));
            currentPerson->id = 1;
            currentPerson->next = NULL;
            currentPerson->type = 0;
            head->next = (struct Person *)currentPerson;
        }
        else
        {
            tmp = (Person *)malloc(sizeof(Person));
            tmp->id = i;
            tmp->next = NULL;
            tmp->type = 0;
            currentPerson->next = (struct Person *)tmp;
            currentPerson = tmp;
        }
    }

    for (j = i; j < NPersoneUbriache + i; j++)
    {
        tmp = (Person *)malloc(sizeof(Person));
        tmp->id = j;
        tmp->next = NULL;
        tmp->type = 1;
        currentPerson->next = (struct Person *)tmp;
        currentPerson = tmp;
    }
    srand(time(NULL));

    /* initialize arr */
    initializePassengersSaliti();
}

int main(int argc, char const *argv[])
{
    int counter = 0;
    pthread_t id;
    Person *currentPassengersOnBoard;
    int response;
    head = NULL;

    startingSetUp();

    while (1)
    {
        currentPassengersOnBoard = chooseRandomPassengers();
        response = pthread_create(&id, NULL, DoATrip, currentPassengersOnBoard);
        if (response)
        {
            printf("ERRORE!");
            exit(-1);
        }
        pthread_join(id, NULL);
        response = pthread_create(&id, NULL, getBack, currentPassengersOnBoard);
        if (response)
        {
            printf("ERRORE!");
            exit(-1);
        }
        pthread_join(id, NULL);
        for (response = 0; response < NPersoneSane+NPersoneUbriache; response++)
        {
            if(passengersSaliti[response] == -1){
                counter++;
            }
        }

        if(counter == 0){
            printf("\n\ninitialize queue\n\n");
            initializePassengersSaliti();
            currentPosInArr = 0;
        }

        counter = 0;
        
    }

    return 0;
}
