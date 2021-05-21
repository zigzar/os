#include <iostream>
#include <pthread.h>
#include <unistd.h>

void* threadFunc1(void* flag)
{
	while(*((bool*)flag))
	{
		std::cout << "1 ";
		std::cout.flush();
		sleep(1);
	}
	return 0;
}

void* threadFunc2(void* flag)
{
	while(*((bool*)flag))
	{
		std::cout << "2 ";
		std::cout.flush();
		sleep(2);
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
	std::cout << "Press Enter to stop program:" << std::endl;
	pthread_create(&th1, NULL, threadFunc1, &isActive1);
	pthread_create(&th2, NULL, threadFunc2, &isActive2);
	std::cin.get();
	isActive1 = false;
	isActive2 = false;
	pthread_join(th1, (void**)&statusPtr1);
	pthread_join(th2, (void**)&statusPtr1);
	return 0;
}