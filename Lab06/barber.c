#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int barberSleeping = 1;
pthread_mutex_t barberSleepingMutex = PTHREAD_MUTEX_INITIALIZER;

int availableChairs;
pthread_mutex_t availableChairsMutex = PTHREAD_MUTEX_INITIALIZER;

int customersProcessed = 0;
pthread_mutex_t customersProcessedMutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t barberAvailableCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t customerArrivedCond = PTHREAD_COND_INITIALIZER;

void barber(int numChairs, int numCustomers)
{
    // Loop until all customers have been processed (either given a haircut or left.)
    pthread_mutex_lock(&customersProcessedMutex);
    while (customersProcessed < numCustomers)
    {
        pthread_mutex_unlock(&customersProcessedMutex);

        // Check if there are any customers waiting.
        pthread_mutex_lock(&availableChairsMutex);
        // No customers are in the waiting room.
        if (availableChairs == numChairs)
        {
            pthread_mutex_unlock(&availableChairsMutex);

            // Mark that the barber is sleeping
            pthread_mutex_lock(&barberSleepingMutex);
            barberSleeping = 1;

            printf("Barber sleeping, as no customers waiting.\n");
            // Wait until a customer arrives.
            pthread_cond_wait(&customerArrivedCond, &barberSleepingMutex);

            // Once customer arrives, barber is awake.
            barberSleeping = 0;
            pthread_mutex_unlock(&barberSleepingMutex);

            printf("Barber starting haircut...\n");
            sleep(2);
            printf("Barber completed haircut!\n");
        }
        // Customers are waiting in the waiting room.
        else
        {
            pthread_mutex_unlock(&availableChairsMutex);

            // Signal to a waiting customer that the barber is available.
            printf("Barber calls in the next customer...\n");
            pthread_cond_signal(&barberAvailableCond);

            printf("Barber starting haircut...\n");
            sleep(2);
            printf("Barber completed haircut!\n");
        }
        pthread_mutex_lock(&customersProcessedMutex);
    }
    pthread_mutex_unlock(&customersProcessedMutex);
    printf("Barber exited!\n");
}

void *customer(void *param)
{
    int id = ((int *)param)[0];
    printf("Customer %d created.\n", id);
    while (1)
    {
        sleep(1);
        if (rand() % 2 == 0)
        {
            printf("Customer %d arrived.\n", id);
            // Check if barber is sleeping
            pthread_mutex_lock(&barberSleepingMutex);
            if (barberSleeping == 1)
            {
                printf("Customer %d saw that barber was sleeping, so waking up barber...\n", id);
                pthread_mutex_unlock(&barberSleepingMutex);

                // If barber is sleeping, wake the barber up via the customerArrivedCond.
                pthread_cond_signal(&customerArrivedCond);

                printf("Customer %d woke up the barber and is getting their haircut!\n", id);
                break;
            }
            pthread_mutex_unlock(&barberSleepingMutex);

            // Otherwise, check if there are any available chairs.
            pthread_mutex_lock(&availableChairsMutex);
            printf("Customer %d saw that barber is busy, so looking for available chair...\n", id);
            if (availableChairs > 0)
            {
                // Take the available chair.
                availableChairs--;
                printf("Customer %d entered the waiting room, so %d chairs are available\n", id, availableChairs);
                pthread_mutex_unlock(&availableChairsMutex);

                // Wait until the barber signals that they are available
                pthread_mutex_lock(&barberSleepingMutex);
                pthread_cond_wait(&barberAvailableCond, &barberSleepingMutex);
                printf("Customer %d was called in, and is getting their haircut!\n", id);
                pthread_mutex_unlock(&barberSleepingMutex);

                // Leave the waiting room
                pthread_mutex_lock(&availableChairsMutex);
                availableChairs++;
                printf("Customer %d left the waiting room, so %d chairs are available\n", id, availableChairs);
                pthread_mutex_unlock(&availableChairsMutex);

                break;
            }
            pthread_mutex_unlock(&availableChairsMutex);

            // Otherwise, there are no available chairs.
            printf("Customer %d saw no available chairs, so they left the shop.\n", id);
            break;
        }
    }

    // The customer has been resolved, so increment the number of customers that have been processed.
    pthread_mutex_lock(&customersProcessedMutex);
    customersProcessed++;
    pthread_mutex_unlock(&customersProcessedMutex);

    // Exit
    pthread_exit(NULL);
}

int main()
{
    int numChairs;
    // Loop until we receive a valid number of chairs
    while (1)
    {
        printf("Input how many waiting room chairs that you want: ");
        scanf("%d", &numChairs);
        printf("\n");

        if (numChairs >= 1)
        {
            break;
        }
        printf("Invalid number of chairs.\n");
    }
    availableChairs = numChairs;

    int numCustomers;
    // Loop until we receive a valid number of customers
    while (1)
    {
        printf("Input how many customers that you want: ");
        scanf("%d", &numCustomers);
        printf("\n");

        if (numCustomers >= 1)
        {
            break;
        }
        printf("Invalid number of customers.\n");
    }

    // Initialize space for each thread
    pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * numCustomers);
    int *ids = (int *)malloc(sizeof(int) * numCustomers);
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    // Create a thread for each customer
    for (int i = 0; i < numCustomers; i++)
    {
        // Give each thread a unique id
        ids[i] = i;
        pthread_create(&threads[i], &attr, customer, &ids[i]);
    }

    barber(numChairs, numCustomers);
    printf("Barber finished giving haircuts!\n");

    // Wait for all threads to complete
    for (int i = 0; i < numCustomers; i++)
    {
        pthread_join(threads[i], NULL);
    }
    printf("All customer threads have completed.\n");

    // Clean-up
    free((pthread_t *)threads);
    free((int *)ids);
    pthread_mutex_destroy(&barberSleepingMutex);
    pthread_mutex_destroy(&availableChairsMutex);
    pthread_mutex_destroy(&customersProcessedMutex);
    pthread_cond_destroy(&barberAvailableCond);
    pthread_cond_destroy(&customerArrivedCond);
}