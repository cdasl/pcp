#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
#define MAX 102
pthread_mutex_t the_mutex;
pthread_cond_t condc, condp;
int buffer = 0;

int getRand()
{
	srand((unsigned)time(NULL));
	return rand()%10 + 1;
}

void *producer(void *ptr)
{
	int i;
	for(i = 1; i <=MAX; i++)
	{
		pthread_mutex_lock(&the_mutex);
		while(buffer >= MAX)
		{
			pthread_cond_wait(&condp, &the_mutex);
		}
		buffer = buffer + 50 >= MAX ? MAX : buffer + 50;
		printf("produce %d\n", buffer);
		pthread_cond_signal(&condc);
		pthread_mutex_unlock(&the_mutex);	
		sleep(2.5);
	}
	pthread_exit(0);
}


void *consumer(void *ptr)
{
	int i;	
	for(i = 1; i <=MAX; i++)
	{
		pthread_mutex_lock(&the_mutex);
		while(buffer <= 0)
		{
			pthread_cond_signal(&condp);
			pthread_cond_wait(&condc, &the_mutex);
		}
		int k = getRand();
		buffer = buffer - k <= 0? 0: buffer-k;
		printf("CONSUMED %d, buffer remains %d\n",k, buffer);
		pthread_cond_signal(&condp);
		pthread_mutex_unlock(&the_mutex);	
		sleep(1);
	}
	pthread_exit(0);
}

int main()
{
	pthread_t pro, con1, con2;
	pthread_mutex_init(&the_mutex, 0);
	pthread_cond_init(&condc, 0);
	pthread_cond_init(&condp, 0);
	pthread_create(&con1, 0, consumer, 0);
	pthread_create(&con2, 0, consumer, 0);
	pthread_create(&pro, 0, producer, 0);
	pthread_join(pro, 0);
	pthread_join(con1, 0);
	pthread_join(con2, 0);
	pthread_cond_destroy(&condc);
	pthread_cond_destroy(&condp);
	pthread_mutex_destroy(&the_mutex);
}
