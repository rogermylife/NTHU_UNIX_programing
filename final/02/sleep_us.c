#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/time.h>
#include<errno.h>
void microseconds_sleep(unsigned long uSec)
{ 
    struct timeval tv; 
    tv.tv_sec=1; 
    tv.tv_usec=0; 
    int err; 
    //do{ 
    err=select(0,NULL,NULL,NULL,&tv); 

    //}while(err<0 && errno==EINTR); 
}
void record()
{
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);
    microseconds_sleep(1000000);
    gettimeofday(&t2, NULL);
    long int elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000000.0;
    elapsedTime += (t2.tv_usec - t1.tv_usec);
    printf("sleep %ld us \n",elapsedTime);
}
int main(int argc, const char *argv[])
{
    for (int i = 0; i < 5; i++) {
        record();
    }
    return 0;
}
