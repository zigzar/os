#include <iostream>
#include <pthread.h>
#include <unistd.h>
typedef struct args_tag {
	bool isActive;
	int value;
} args_t;

void* threadFunc1(void* args)
{
	args_t* pArgs = (args_t*)args;
	while(pArgs->isActive)
	{
		std::cout << "1 ";
		std::cout.flush();
		sleep(1);
	}
	return 0;
}

void* threadFunc2(void* args)
{
	args_t* pArgs = (args_t*)args;
	while(pArgs->isActive)
	{
		std::cout << "2 ";
		std::cout.flush();
		sleep(2);
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
	}
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	pthread_join(aborter, NULL);
	return 0;
}