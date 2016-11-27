#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#include<sys/wait.h>
#define DEFAULT 1000

//char command[100];
char *args[100];
int argc;
bool background;
int psTable[DEFAULT]={0};
int  psIndex;
void splitInput(char *input)
{
    char *pch;
    pch = strtok(input," \n");
    //strcpy(command,pch);
    //pch = strtok(NULL," \n");
    //if(pch==NULL)
    //    printf("pch NULL\n");
    while(pch!=NULL)
    {
        if(strcmp(pch,"&")==0)
        {
            background=true;
            pch = strtok(NULL," \n");
            continue;
        }
        if(pch[strlen(pch)-1]=='&')
        {
            background=true;
            pch[strlen(pch)-1]='\0';
        }
        args[argc] =(char*)malloc(strlen(pch)+1);
        strcpy(args[argc++],pch);
        pch = strtok(NULL," \n");
    }
    if(args[0]!=NULL&&args[argc-1][strlen(args[argc-1])]=='&')
        background=true;
    //strcpy(args[argc++],"0>/dev/null");
    //printf("check\n");
    //strcpy(args,input+strlen(command)+1);
    //printf("test %s %s\n",command,input+strlen(command)+1);
    args[argc]=NULL;
    //args[argc]=(char*)malloc(1);
    //args[argc][0]=0;
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
    //printf("command : [%s]\n",command);
    for(int i=0;i<argc;i++)
    {
        printf("args[%d] : [%s]\n",i,args[i]);
    }
    //if(strcmp(args[argc],"\0")!=0)
    //    printf("args[argc]!=\\0\n");
    if (args[argc]!=NULL)
        printf("args[argc]!=NULL\n");
}

int main()
{
    char pwd[DEFAULT];
    char input[DEFAULT];
    for (int i=0;i<DEFAULT;i++)
    {
        psTable[0]=false;
    }
    psTable[1]=true;
    psIndex=0;
    while(true)
    {
        background=false; 
        argc=0;
        getcwd(pwd,DEFAULT);
        printf("$ ");
        fflush(stdout);
        fgets(input,DEFAULT,stdin);
        //input[strlen(input)-1]='\0';
        splitInput(input);
        if(args[0]==NULL)
            continue;
        if(strcmp(args[0],"cd")==0)
        {
            if(args[0]==NULL)
                printf("no directory");
            else
            {
                int ret = chdir(args[1]);
                //printf("chdir [%d] [%s] \n",ret,args[1]);
                if(ret!=0)
                    printf("cd: %s: No such file or directory\n",args[1]);
            }
        }
        else if(strcmp(args[0],"pwd")==0)
        {
            printf("%s\n",pwd);
        }
        else
        {
            pid_t pid=-1;
            int status;
            if((pid=fork())<0)
            {
                printf("fork error");
            }
            else if(pid==0)
            {
                if(background==true)
                {
                    pid_t pid2=-1;
                    if((pid2=fork())<0)
                        printf("fork error");
                    else if(pid2 > 0)
                        exit(87);
                    //showExecInfo();
                    FILE *trash;
                    if(background==true)
                    {
                        
                        //trash = fopen("/dev/null","r");
                        //fprintf(trash," ");
                        //dup2(fileno(trash),0);
                        close(0);
                    }
                    if(execvp(args[0],args)<0)
                        printf("%s: command not found\n",args[0]);
                    if(background==true)
                    {
                        //fclose(trash);
                        //remove("temp.XDDDDDDDDDDDDDDDDDDDDDDDDDDDDD");
                    }
                    exit(7);
                }
                if(execvp(args[0],args)<0)
                    printf("%s: command not found\n",args[0]);
                exit(7);
                 
            }
            else
            {
                if(background==true)
                    waitpid(pid,NULL,0);
                if(background==false&&waitpid(pid,NULL,0)!=pid)
                    printf("waitpid error");
                else if(background==true)
                {
                    /*
                    printf("[%d] %d\n",++psIndex,pid);
                    psTable[psIndex]=pid;
                    */

                }
                /*
                for(int i=1;i<=psIndex&&pid!=psTable[psIndex];++i) 
                {
                    printf("waitpid(psTable[%d],NULL,WNOHANG\n",i);
                    if(waitpid(psTable[i],NULL,WNOHANG)==0)
                        printf("[%d] processing \n",i);
                    else
                    {
                        printf("[%d] Done\n",i);
                        psTable[psIndex]=0;
                    }
                }
                while(psTable[psIndex]==0&&psIndex>1) --psIndex;
                */
            }
        }
        flushArgs();
    }
}

