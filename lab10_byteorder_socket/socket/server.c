#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include <netinet/in.h>
#include<netdb.h>
#include<unistd.h>
#include<strings.h>
#include<sys/sysinfo.h>
#include<signal.h>
void sig_fork(int signo)
{
        pid_t pid;
        int stat;
        pid=waitpid(0,&stat,WNOHANG);
        printf("child died\n");
}
void process(int clientFD)
{
    int n;
    char buffer[256];
    bzero(buffer,256);
    //n=write(clientFD,"GOGOGO\n",7);
    struct sysinfo si;
    while(1)
    {
        sysinfo(&si);
        sprintf(buffer,"ps count %d\n",si.procs);
        n=write(clientFD,buffer,strlen(buffer));
        sleep(1);
    }
}
int main(int argc ,char *argv[])
{
    signal (SIGCHLD, sig_fork);
    struct sockaddr_in sin;
    int pid;
    int serverFD = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    int reuse=1;
    if(argc<2)
    {
        printf("./server <port>\n");
        exit(0);
    }
    if(serverFD<0)
    {
        printf("socket error\n");
        exit(1);
    }
    if (setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &reuse,sizeof(int)) < 0)
    {
        printf("set socket error");
        exit(1);
    }
    bzero(&sin,sizeof(sin));
    sin.sin_family=AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port=htons(atoi(argv[1]));
    if(bind(serverFD,&sin,sizeof(sin))<0)
    {
        printf("bind error\n");
        exit(1);
    }
    
    listen(serverFD,20);
    while(1)
    {
        struct sockaddr_in sout;
        int clen = sizeof(sout);
        int clientFD = accept(serverFD,&sout,&clen);
        if(clientFD < 0)
            printf("error on accept\n");
        printf ("accept a child\n");
        pid=fork();
        if(pid==0)
        {
            close(serverFD);
            process(clientFD);
            exit(0);
        }
        close(clientFD);
    }
}
