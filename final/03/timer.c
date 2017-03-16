#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<signal.h>
pthread_t array[1000];


int index = 0;


void *threadSig(void *arg)
{
    int sec=*(int*)arg;
    sleep(sec-1);
    alarm(1);
    //printf("%lu call alarm %d \n",pthread_self(),sec);
}
void setAlarm(int sec)
{
    int *number = (int*)malloc(sizeof(int));
    *number=sec;
    pthread_create(&array[index++],NULL,threadSig,number);
    index++;
}
void clearAlarm()
{
    for (int i = 0; i < index; i++) {
        pthread_cancel(array[i]);
    }
    index=0;
}
void sigHandler(int sig)
{
    if(sig==SIGALRM)
        printf("alarm!!!\n");
}
int main(int argc, const char *argv[])
{
    signal(SIGALRM,sigHandler);
    setAlarm(2);
    sleep(1);
    setAlarm(6);
    sleep(1);
    setAlarm(3);
    sleep(4);
    clearAlarm();
    return 0;
}
