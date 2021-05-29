#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <string.h>

#define READ_ERROR -1
#define BUFFER_SIZE 128

int fd[2];

void sleep(int ms)
{
    timespec tp;
    tp.tv_sec = ms / 1000;
    tp.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&tp, NULL);
}

void *sender(void *flag)
{
    char buf[BUFFER_SIZE];
    std::string messageStart = "I ";
    std::string messageCenter = "so ";
    std::string messageEnd = "hate C++ and C";
    std::string message;
    int hateRate = 1;
    while (*((bool *)flag))
    {
        message = messageStart;
        for (int i = 0; i < hateRate; i++)
            message += messageCenter;
        message += messageEnd;
        strcpy(buf, message.c_str());
        write(fd[1], buf, BUFFER_SIZE);
        hateRate++;
        sleep(500);
    }
    return 0;
}

void *receiver(void *flag)
{
    char buf[BUFFER_SIZE];

    while (*((bool *)flag))
    {
        if (read(fd[0], &buf, BUFFER_SIZE) != -1)
        {
            std::cout << buf << std::endl;
        }
        else
        {
            std::cout << "Can't receive" << std::endl;
        }

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
    pipe(fd);
    fcntl(fd[0], F_SETFL, O_NONBLOCK);
    //pipe2(fd, O_NONBLOCK);

    std::cout << "Press Enter to stop program:" << std::endl;
    pthread_create(&th1, NULL, sender, &isActive1);
    pthread_create(&th2, NULL, receiver, &isActive2);
    std::cin.get();
    isActive1 = false;
    isActive2 = false;
    pthread_join(th1, (void **)&statusPtr1);
    pthread_join(th2, (void **)&statusPtr1);

    close(fd[0]);
    close(fd[1]);
    return 0;
}