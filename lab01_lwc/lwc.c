#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#define BUFFER_SIZE 16*1024

static long long int total_lines;
static long long int total_words;
static long long int total_bytes;

static bool print_lines,print_words,print_bytes;

void wc(char* const filename)
{
    printf("input file %s\n",filename);
    int fd = open(filename,O_RDONLY);
    if(fd==-1)
    {
        printf("wc: %s: No such file or directory\n",filename);
        exit(2);
    }
    bool inWord = false;
    int  linepos = 0;
    int bytesRead =0;
    char buf[BUFFER_SIZE+1];
    long long int bytes = 0;
    while((bytesRead=read(fd,buf,BUFFER_SIZE)) > 0 )
    {
        const char *p = buf;
        bytes += bytesRead;
    }
    printf("bytes %lld\n",bytes);
    if(close(fd)!=0)
    {
        printf("wc: %s: close file error\n",filename);
        exit(2);
    }

}


int main(int argc,char **argv)
{
    print_lines=print_words=print_bytes=false;
    total_lines=total_words=total_bytes=0;
    for(int i=1;i<argc;i++)
    {
        int l=strlen(argv[i]);
        if(argv[i][0]=='-')
        {
            for(int j=1;j<l;j++)
            {
                switch (argv[i][j])
                {
                    case 'l':
                        print_lines=true;
                        break;
                    case 'w':
                        print_words=true;
                        break;
                    case 'c':
                        print_bytes=true;
                        break;
                    default:
                        printf("wc: invalid option -- \'%c\'\n\
Try \'wc --help\' for more information.\n",argv[i][j]);
                        return 2;
                }
            }
            printf("option %s  len %d\n",argv[i],l);
        }
    }

    if(!(print_lines||print_words||print_bytes))
        print_lines=print_words=print_bytes=true;
    for(int i=1;i<argc;i++)
    {
        if(argv[i][0]!='-')
            wc(argv[i]);
    }
}
