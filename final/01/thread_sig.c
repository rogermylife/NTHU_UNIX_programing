#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<signal.h>

pthread_t ntid[3];
pthread_t parent;
void sigHandler(int sig)
{
    pthread_t tid=pthread_self();
    //printf("%lu get signal %d\n",tid,sig);
    if(tid==parent)
    {
        if(sig==SIGINT)
            tid=ntid[0];
        else if(sig==SIGTERM)
            tid=ntid[1];
        else if(sig==SIGUSR1)
            tid=ntid[2];
        pthread_kill(tid,sig);
        return ;
    }
        
    int number=-1;
    for(int i=0;i<3;i++)
    {
        //printf("%lld %lld\n",tid,ntid[i]);
        if(tid==ntid[i])
        {
            number=i+1;
            break;
        }
    }
    if(sig==SIGINT)
        printf("Signal handling thread %d got signal SIGINT\n",number);
    if(sig==SIGTERM)
    {
        printf("Signal handling thread %d got signal SIGTERM\n",number);
        exit(0);
    }
    if(sig==SIGUSR1)
        printf("Signal handling thread %d got signal SIGUSR1\n",number);

}

void *threadSig(void *arg)
{
    int number=*(int*)arg;

    //if(number ==2)
    //    sleep(2);

    //printf("HIHI %d\n",number);

    while(1)
    {
        pause();
    }

}

int main(int argc, const char *argv[])
{
    if (signal(SIGINT, sigHandler) == SIG_ERR||
        signal(SIGTERM, sigHandler) == SIG_ERR||
        signal(SIGUSR1, sigHandler) == SIG_ERR)
        printf("can not set sigHandler\n");
    //printf("%ld\n",pthread_self());
    parent = pthread_self();
    for (int i = 0; i < 3; i++) {
        int *number = (int*)malloc(sizeof(int));
        *number=i+1;
        int err = pthread_create(&ntid[i],NULL,threadSig,number);
        if(err !=0)
        {
            printf("cannot create pthread\n");
            exit(1);
        }
    }
    /*
    for (int i = 0; i < 3; i++) {
        printf("%lu \n",ntid[i]);
    }
    */
    while(1)
        pause();
    return 0;
}
