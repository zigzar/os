#include <iostream>
#include <pthread.h>
typedef struct args_tag {
	bool isActive;
	int value;
} args_t;

void sleep(int ms) {
    timespec tp;
    tp.tv_sec = ms / 1000;
    tp.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&tp, NULL);
}

void* threadFunc1(void* args)
{
	args_t* pArgs = (args_t*)args;
	while(pArgs->isActive)
	{
		pArgs->value = 1;
		sleep(300);
	}
	return 0;
}

void* threadFunc2(void* args)
{
	args_t* pArgs = (args_t*)args;
	while(pArgs->isActive)
	{
		pArgs->value = 2;
		sleep(200);
	}
	return 0;
}

void* aborterFunc(void* args)
{
	args_t* pArgs = (args_t*)args;
	std::cin.get();
	pArgs->isActive = false;
	return 0;
}

int main()
{
	args_t arg;
	arg.isActive = true;
	pthread_t th1, th2, aborter;
	std::cout << "Press Enter to stop program:" << std::endl;
	pthread_create(&th1, NULL, threadFunc1,  (void*)&arg);
	pthread_create(&th2, NULL, threadFunc2,  (void*)&arg);
	pthread_create(&aborter, NULL, aborterFunc,  (void*)&arg);
	while (arg.isActive)
	{
		std::cout << arg.value << " ";
		std::cout.flush();
		sleep(150);
	}
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	pthread_join(aborter, NULL);
	return 0;
}