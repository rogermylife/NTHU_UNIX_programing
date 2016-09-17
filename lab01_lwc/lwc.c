#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<ctype.h>
#define BUFFER_SIZE 16*1024
#define NUMBER_WIDTH 5

static long long int totalLines;
static long long int totalWords;
static long long int totalBytes;

static bool printLines,printWords,printBytes;

void writeCount(char* const fileName,long long int lines,long long int words,long long int bytes)
{
    static char const FORMAT_INT[]="%*lld";
    if(printLines)
        printf(FORMAT_INT,NUMBER_WIDTH,lines);
    if(printWords)
        printf(FORMAT_INT,NUMBER_WIDTH,words);
    if(printBytes)
        printf(FORMAT_INT,NUMBER_WIDTH,bytes);
    printf(" %s\n",fileName);
}

void wc(char* const fileName)
{
    //printf("input file %s\n",fileName);
    int fd = open(fileName,O_RDONLY);
    if(fd==-1)
    {
        printf("wc: %s: No such file or directory\n",fileName);
        exit(2);
    }
    bool inWord = false;
    int  linePos = 0;
    int bytesRead =0;
    char buf[BUFFER_SIZE+1];
    long long int bytes = 0;
    long long int words = 0;
    long long int lines = 0;
    while((bytesRead=read(fd,buf,BUFFER_SIZE)) > 0 )
    {
        const char *p = buf;
        bytes += bytesRead;
        do
        {
            switch (*p++)
            {
                case '\n':
                    ++lines;
                case '\r'://carriage return
                case '\f'://form feed
                    linePos=0;
                    goto word_separator;
                case '\t':
                    linePos += 8-(linePos %8);
                    goto word_separator;
                case ' ':
                    linePos++;
                word_separator:
                    words += inWord;
                    inWord = false;
                    break;
                default:
                    if(isprint(p[-1]))
                    {
                        linePos++;
                        if(isspace(p[-1]))
                            goto word_separator;
                        inWord = true;
                    }
                    break;
            }
            //printf("p %d inWord %d\n",*(p-1),inWord);
        }while(--bytesRead);
    }
    words +=inWord;
    //printf("bytes %lld words %lld lines %lld\n",bytes,words,lines);
    writeCount(fileName,lines,words,bytes);
    totalLines +=lines;
    totalWords +=words;
    totalBytes +=bytes;
    if(close(fd)!=0)
    {
        printf("wc: %s: close file error\n",fileName);
        exit(2);
    }

}


int main(int argc,char **argv)
{
    printLines=printWords=printBytes=false;
    totalLines=totalWords=totalBytes=0;
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
                        printLines=true;
                        break;
                    case 'w':
                        printWords=true;
                        break;
                    case 'c':
                        printBytes=true;
                        break;
                    default:
                        printf("wc: invalid option -- \'%c\'\n\
Try \'wc --help\' for more information.\n",argv[i][j]);
                        return 2;
                }
            }
            //printf("option %s  len %d\n",argv[i],l);
        }
    }

    if(!(printLines||printWords||printBytes))
        printLines=printWords=printBytes=true;
    int nFiles=0;
    for(int i=1;i<argc;i++)
    {
        if(argv[i][0]!='-')
        {
            wc(argv[i]);
            ++nFiles;
        }
    }
    if(nFiles>1)
        writeCount("Total",totalLines,totalWords,totalBytes);
}
