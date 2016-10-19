#include<stdio.h>
#include<stdlib.h>
#include<sys/times.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#define MAX 1024*1024*5
int main(int argc,char** argv)
{
    struct tms *start = (struct tms*)malloc(sizeof(struct tms));
    struct tms *end = (struct tms*)malloc(sizeof(struct tms));
    clock_t startclock = clock();
    times(start);
    FILE *in,*out;
    //int in,out;
    int size = atoi(argv[1]);
    //in=open("XDD",O_RDONLY);
    in=fopen("XDD","r");
    char *buf = (char*)malloc(MAX);
    char *buff = (char*)malloc(size);
    //out = fileno(fmemopen(buf,MAX+1,"w"));
    out = fmemopen(buf,MAX,"w");
    int readSize;
    //while((readSize=read(in,buff,size))>0)
    //int c;
    //while((c=getc(in))!=EOF)
    while((fgets(buff,size,in))!=NULL)
    {
        //write(out,buff,readSize);
        //putc(c,out);
        //fputc(c,out);
        //fputc(c,out);
        fputs(buff,out);
    }
    times(end);
    clock_t endclock = clock();
    printf("uuu   %d\n",end->tms_utime-start->tms_utime);
    printf("sss   %d\n",end->tms_stime-start->tms_stime);
    printf("aaa   %lf\n",((double)endclock - (double)startclock) / CLOCKS_PER_SEC);
}
