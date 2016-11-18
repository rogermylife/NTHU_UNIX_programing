#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#include<sys/wait.h>
#define DEFAULT 1000

char command[100];
char *args[100];
int argc;

void splitInput(char *input)
{
    char *pch;
    pch = strtok(input," \n");
    strcpy(command,pch);
    pch = strtok(NULL," \n");
    while(pch!=NULL)
    {
        args[argc] =(char*)malloc(strlen(pch)+1);
        strcpy(args[argc++],pch);
        pch = strtok(NULL," \n");
    }
    //strcpy(args,input+strlen(command)+1);
    //printf("test %s %s\n",command,input+strlen(command)+1);
    //args[argc]=NULL;
    args[argc]=(char*)malloc(1);
    args[argc][0]=0;
}

void flushArgs()
{
    for(int i=0;i<argc;i++)
    {
        free(args[i]);
        args[i]=NULL;
    }
    argc=0;
}

void showExecInfo()
{
    printf("command : [%s]\n",command);
    for(int i=0;i<argc;i++)
    {
        printf("args[%d] : [%s]\n",i,args[i]);
    }
    if(strcmp(args[argc],"\0")!=0)
        printf("args[argc]!=\\0\n");
}

int main()
{
    char pwd[DEFAULT];
    char input[DEFAULT];
    
    while(true)
    {
        argc=0;
        getcwd(pwd,DEFAULT);
        printf("%s $",pwd);
        fgets(input,DEFAULT,stdin);
        //input[strlen(input)-1]='\0';
        splitInput(input);
        if(strcmp(command,"cd")==0)
        {
            if(args[0]==NULL)
                printf("no directory");
            else
            {
                int ret = chdir(args[0]);
                printf("chdir [%d] [%s] \n",ret,args[0]);
            }
        }
        if(strcmp(command,"pwd")==0)
        {
            printf("%s\n",pwd);
        }
        else
        {
            pid_t pid;
            int status;
            if((pid=fork())<0)
            {
                printf("fork error");
            }
            else if(pid==0)
            {
                showExecInfo();
                if(execvp(command,args)<0)
                    printf("exevp error");
                exit(7); 
            }
            else
            {
                if(waitpid(pid,NULL,0)!=pid)
                    printf("waitpid error");
            }
        }
        flushArgs();
    }
}
