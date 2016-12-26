#include<stdio.h>
#include<stdlib.h>
#include<netdb.h>
#include<netinet/in.h>
#include<string.h>

int main()
{
    int clientFD = socket(AF_INET,SOCK_STREAM,0);
    struct hostent *server = gethostbyname("localhost");
    struct sockaddr_in cout;
    char buf[256];
    if(server==NULL)
    {
        printf("no such host\n");
        exit(0);
    }
    bzero(&cout,sizeof(cout));
    cout.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&cout.sin_addr.s_addr, server->h_length);
    cout.sin_port = htons(9999);

    if(connect(clientFD,&cout,sizeof(cout))<0)
    {
        printf("connect error\n");
        exit(1);
    }
    bzero(buf,256);
    int n=read(clientFD,buf,255);
    if(n<0)
    {
        printf("read error");
    }
    print ("%s\n",buf);
}
