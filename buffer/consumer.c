#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#define _SIZE_ 64;

//consumer lock
pthread_mutex_t con_lock;
//product lock
pthread_mutex_t pro_lock;

sem_t sem_product;
sem_t sem_consume;
int buf[64];
int con_index = 0;
int pro_index = 0;

void *consumer_run(void* arg)
{
	while(1)
	{
		sem_wait(&sem_consume);
		pthread_mutex_lock(&con_lock);
		int data = buf[con_index];
		printf("consume done ... data :%d\n",data);
		con_index++;
		con_index %= _SIZE_;
		pthread_mutex_unlock(&con_lock);
		sem_post(&sem_product);
		//sleep(1);
	}

}
void *producter_run(void* arg)
{
	while(1)
	{
		sem_wait(&sem_product);
		int data = rand()%1234;
		pthread_mutex_lock(&pro_lock);
		buf[pro_index] = data;
		printf("proudct done ... data:%d\n",data);
		pro_index++;
		pro_index %= _SIZE_;
		pthread_mutex_unlock(&pro_lock);
		sem_post(&sem_consume);
		sleep(1);
	}
}
int main()
{
	pthread_t id1,id2, id3, id4;
	sem_init(&sem_product,0,64);
	sem_init(&sem_consume,0,0);
	pthread_create(&id1,NULL,consumer_run,NULL);
	pthread_create(&id2,NULL,consumer_run,NULL);
	pthread_create(&id3,NULL,producter_run,NULL);
	pthread_create(&id4,NULL,producter_run,NULL);


	sem_destroy(&sem_product);
	sem_destroy(&sem_consume);
	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	pthread_join(id3,NULL);
	pthread_join(id4,NULL);
	return 0;
}
