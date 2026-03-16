#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

#define SIZE 10

pthread_cond_t not_empty;
pthread_cond_t not_full;
pthread_mutex_t lock;

int rear = 0, front = 0, cnt = 0;
int Queue[SIZE];

void enqueue(int data)
{
    Queue[rear] = data;
    rear = (rear + 1) % SIZE;
    cnt++;
}

int dequeue(void)
{
    int data = Queue[front];
    front = (front + 1) % SIZE;
    cnt--;
    return data;
}

void *producer(void *v)
{
    for(int i = 1; i <= SIZE; i++)
    {
        pthread_mutex_lock(&lock);

        while(cnt == SIZE)
            pthread_cond_wait(&not_full, &lock);

        enqueue(i);
        printf("Produced = %d\n", i);

        pthread_cond_signal(&not_empty);

        pthread_mutex_unlock(&lock);
        sleep(1);
    }
    return NULL;
}

void *consumer(void *v)
{
    int data;

    for(int i = 1; i <= SIZE; i++)
    {
        pthread_mutex_lock(&lock);

        while(cnt == 0)
            pthread_cond_wait(&not_empty, &lock);

        data = dequeue();
        printf("Consumed = %d\n", data);

        pthread_cond_signal(&not_full);

        pthread_mutex_unlock(&lock);
        sleep(2);
    }
    return NULL;
}

int main()
{
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&not_empty, NULL);
    pthread_cond_init(&not_full, NULL);

    pthread_t t1, t2;

    pthread_create(&t1, NULL, producer, NULL);
    pthread_create(&t2, NULL, consumer, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&not_full);

    return 0;
}
