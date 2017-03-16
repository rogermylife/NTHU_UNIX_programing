#include <endian.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>





int main()
{
    #if __BYTE_ORDER == __LITTLE_ENDIAN
        printf("little endian\n");
    #elif __BYTE_ORDER == __BIG_ENDIAN
        printf("big endian\n");
    #elif __BYTE_ORDER == __PDP_ENDIAN
        printf("pdp endian\n");
    #else
        printf("no order\n");
    #endif

    if(100==htonl(100))
        printf("BIG ENDIAN\n");
    else
        printf("LITTLE ENDIAN\n");


    int a=0x41000042;
    if(strcmp(&a,"B")==0)
        printf("Little endian\n");
    else if(strcmp(&a,"A")==0)
        printf("Big endian\n");
}

