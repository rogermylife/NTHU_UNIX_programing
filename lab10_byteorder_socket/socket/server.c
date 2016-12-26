#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include <netinet/in.h>
#include<netdb.h>
#include<unistd.h>
#include<strings.h>

void process(int clientFD)
{
    int n;
    char buffer[256];
    bzero(buffer,256);
    n=write(clientFD,"GOGOGO\n",7);
}
int main()
{
    struct sockaddr_in sin;
    int pid;
    int serverFD = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    bzero(&sin,sizeof(sin));
    sin.sin_family=AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port=htons(9999);
    bind(serverFD,&sin,sizeof(sin));
    listen(serverFD,20);
    while(1)
    {
        struct sockaddr_in sout;
        int clen = sizeof(sout);
        int clientFD = accept(serverFD,&sout,&clen);
        if(clientFD < 0)
            printf("error on accept\n");
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
