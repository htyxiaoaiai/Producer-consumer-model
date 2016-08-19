#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t lock;
pthread_cond_t need_product;

typedef struct node
{
	int data;
	struct node* next;
}node,*node_p,**node_pp;

node_p head = NULL;

//申请节点
static node_p alloc_node(int data)
{
	node_p tmp = (node_p)malloc(sizeof(node));
	if(tmp)
	{
		tmp->data = data;
		tmp->next = NULL;
	}
	return tmp;
}
//初始化链表
static void inint_list(node_pp _head)
{
	*_head = alloc_node(0);
	(*_head)->next = NULL;
}
//判断链表是否位空
static int isempty(node_p _head)
{
	return _head->next == NULL;
}
//头插
static void push_front(node_p _head,int data)
{
	node_p new_node = alloc_node(data);
	new_node->next = _head->next;
	_head->next = new_node;
}
//头删
static void pop_front(node_p _head)
{
	if(!isempty(head))
	{
		node_p delete = _head->next;
		_head->next = delete->next;
		free(delete);
		delete = NULL;
	}
}
//打印链表
static void print_list(node_p _head)
{
	node_p cur = _head->next;
	while(cur)
	{
		printf("%d :",cur->data);
		cur = cur->next;
	}
	printf("\n");
}
void *consumer_run(void* arg)
{
	while(1)
	{

		pthread_mutex_lock(&lock);
		while(isempty(head))
		{

			printf("consumer done ... call him\n");
			pthread_cond_wait(&need_product,&lock);
		}
		node_p tmp = head->next;
		int data = tmp->data;
		pop_front(head);
		printf("consumer :%d \n",data);
		pthread_mutex_unlock(&lock);
		sleep(1);
	}

}
void *producter_run(void* arg)
{
	while(1)
	{
		pthread_mutex_lock(&lock);
		int data = rand()%1234;
		push_front(head,data);
		printf("producter :%d\n",data);
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&need_product);
		sleep(5);
	}

}
int main()
{
	inint_list(&head);
	pthread_t id1,id2;
	pthread_mutex_init(&lock,NULL);
	pthread_cond_init(&need_product,NULL);
	pthread_create(&id1,NULL,consumer_run,NULL);
	pthread_create(&id2,NULL,producter_run,NULL);


	/*int i = 0;
	for(; i<10; i++)
	{
		push_front(head,i);
		print_list(head);
		sleep(1);
	}
	for(; i>0; i--)
	{
		pop_front(head);
		print_list(head);
		sleep(1);
	}*/
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&need_product);
	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	return 0;
}
