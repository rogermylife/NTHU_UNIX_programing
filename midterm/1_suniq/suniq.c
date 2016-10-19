#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#define MAX 4096

bool caseF = false,countF=false,wordF=false;
int wordN=-1;
int ln;
char **lineTable;


void uniq()
{
    char *parrent=lineTable[0];
    int count=1;
    if(wordN==-1)
        wordN==2147483647;
    for(int i=1;i<ln;i++)
    {
        if(caseF)
        {
            if(strncasecmp(parrent,lineTable[i],wordN)==0)
                count++;
            else
            {
                if(countF)
                    printf("%7d %s",count,parrent);
                else
                    printf("%s",parrent);
                count=1;
                parrent = lineTable[i];
            }
        }
        else
        {
            //printf("check %s %s",parrent,lineTable[i]);
            if(strncmp(parrent,lineTable[i],wordN)==0)
                count++;
            else
            {
                if(countF)
                    printf("%7d %s",count,parrent);
                else
                    printf("%s",parrent);
                count=1;
                parrent = lineTable[i];
            }
        }
    }
    if(countF)
        printf("%7d %s",count,parrent);
    else
        printf("%s",parrent);
}

void showTable()
{
    for(int i=0;i<ln;i++)
        printf("%s",lineTable[i]);
    printf("---------------------------------\n");
}

int mystrcmp(const char *s1, const char *s2)
{
    
    if(caseF)
    {
        for ( ; tolower(*s1) == tolower(*s2); s1++, s2++)
	        if (*s1 == '\0')
	            return 0;
        return (tolower((*(unsigned char*)s1) < tolower(*(unsigned char*)s2)) ? -1 : +1);

    }
    
    for ( ; *s1 == *s2; s1++, s2++)
	    if (*s1 == '\0')
	        return 0;
    double fs1=*s1;
    double fs2=*s2;
    if(fs1>=65 &&fs1<=90)
        fs1+=32.5;
    if(fs2>=65 &&fs2<=90)
        fs2+=32.5;
    return (((double)fs1 < (double)fs2) ? -1 : +1);
}


int compare(const void *a,const void *b)
{
    //if(strcmp(a,b)>0)
    //    return 0;
    //return 1;
    return mystrcmp(*(char **)a, *(char **)b);
    //return strcmp(*(char **)b, *(char **)a);
}

void doSort(char *fileName)
{
    char *buf = (char*)malloc(MAX);
    char *line;
    lineTable = (char**)malloc(sizeof(char*)*MAX);
    ln=0;
    FILE* file = fopen(fileName,"r");
    if(file==NULL)
    {
        fprintf(stderr,"unknown file %s\n",fileName);
        exit(1);
    }
    while(fgets(buf,MAX,file)!=NULL)
    {
        int l = strlen(buf);
        if(buf[l-1]=='\n'&&l<MAX)
        {
            //printf("get newline\n");
            //line = buf;
            line = (char*)malloc(l+1);
            strcpy(line,buf);
        }
        else
        {
            line = (char*)malloc(l+1);
            strcpy(line,buf);
            while(fgets(buf,MAX,file)!=NULL)
            {
                line =  (char*)realloc(line,l+1);
                strcat(line,buf);
                printf("realloc %s\n",line);
                if(buf[l-1]=='\n')
                    break;
            }
        }
        if(line==NULL)
        {
            fprintf(stderr,"line malloc error please increase machine memory");
            exit(1);
        }
        lineTable[ln]=line;
        ++ln;
    }
    qsort(lineTable,ln,sizeof(char*),compare);
}
int main(int argc,char **argv)
{
    int optc =0;
    char c;
    char* fileName;
    while((c = getopt (argc, argv, "icw:")) != -1)
    {
		switch(c)
        {
            case 'i':
                caseF=true;
                //printf("get i flag\n");
                break;
            case 'c':
                countF = true;
                //printf("get c flag\n");
                break;
            case 'w':
                wordN = atoi(optarg);
                wordF = true;
                //printf("get w flag %d\n",wordN);
                break;
            case '?':
                if (optopt == 'w')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
                return 1;
            default:
                abort ();        
        }
    }
    if(wordN<0&&wordF)
        fprintf (stderr, "uniq: -%d: invalid number of bytes to compare\n", wordN);

    for(int i=1;i<argc;++i)
    {
        if(argv[i][0]=='-')
            continue;
        fileName = argv[i];
        //printf("get filename %s\n",fileName);
    }
    doSort(fileName);
    //showTable();
    uniq();
}
