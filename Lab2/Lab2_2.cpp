#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define CANT_LOCK 0
#define CANT_UNLOCK -1

sem_t semaphore;

const int NUM_PRINT = 3;

void sleep(int ms)
{
    timespec tp;
    tp.tv_sec = ms / 1000;
    tp.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&tp, NULL);
}

void *threadFunc1(void *flag)
{
    int status;
    while (*((bool *)flag))
    {
        status = sem_trywait(&semaphore);
        if (status != CANT_LOCK)
        {
            for (int i = 0; i < NUM_PRINT; i++)
            {
                sleep(100);
                std::cout << "1 ";
                std::cout.flush();
            }
            std::cout << "\t";
            std::cout.flush();
            if (sem_post(&semaphore) == CANT_UNLOCK)
            {
                exit(1);
            }
        }
        else
            sleep(500);
    }
    return 0;
}

void *threadFunc2(void *flag)
{
    int status;
    while (*((bool *)flag))
    {
        status = sem_trywait(&semaphore);
        if (status != CANT_LOCK)
        {
            for (int i = 0; i < NUM_PRINT; i++)
            {
                sleep(100);
                std::cout << "2 ";
                std::cout.flush();
            }
            std::cout << "\t";
            std::cout.flush();
            if (sem_post(&semaphore) == CANT_UNLOCK)
            {
                exit(1);
            }
        }
        else
            sleep(500);
    }
    return 0;
}

int main()
{
    bool isActive1 = true;
    bool isActive2 = true;
    int statusPtr1;
    int statusPtr2;
    pthread_t th1, th2;

    sem_init(&semaphore, 0, 1);
    std::cout << "Press Enter to stop program:" << std::endl;
    pthread_create(&th1, NULL, threadFunc1, &isActive1);
    pthread_create(&th2, NULL, threadFunc2, &isActive2);
    std::cin.get();

    isActive1 = false;
    isActive2 = false;

    pthread_join(th1, (void **)&statusPtr1);
    pthread_join(th2, (void **)&statusPtr1);

    sem_destroy(&semaphore);

    return 0;
}