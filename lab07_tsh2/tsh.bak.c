#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/mman.h>
#define DEFAULT 1024

//char command[100];
typedef struct job{
    int pid;
    char *command;
}JOB;
char *args[100];
int argc;
bool background;
JOB psTable[DEFAULT];
int  psIndex;
static int *gloPid2;
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

void jobs()
{
    printf("my jobsi\n");
    for(int i=1;i<=psIndex;++i)
    {
        printf("[%d] %s\n",i,psTable[i].command);
    }
}
void checkJobs()
{
    for(int i=1;i<=psIndex;++i) 
    {
        printf("kill(psTable[%d].pid => %d  command =>%s,NULL,WNOHANG\n",i,psTable[i].pid,psTable[i].command);
        //int res=waitpid(psTable[i].pid,NULL,WNOHANG);
        int res=kill(psTable[i].pid,0);
        printf("result %d\n",res);
        if(res==0)
            printf("[%d] processing \n",i);
        else
        {
            printf("[%d] Done\n",i);
            psTable[i].pid=0;
            free(psTable[i].command);
        }
    }
    while(psTable[psIndex].pid==0&&psIndex>0) --psIndex;

}
int main()
{
    char pwd[DEFAULT];
    char input[DEFAULT];
    gloPid2 = mmap(NULL, sizeof *gloPid2, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    //psTable[1]=true;
    psIndex=0;
    while(true)
    {
        *gloPid2 = -1;
        background=false; 
        argc=0;
        char *command;
        getcwd(pwd,DEFAULT);
        checkJobs();
        printf("%d %d $ ",getpid(),getpgid(0));
        fflush(stdout);
        fgets(input,DEFAULT,stdin);
        command = (char*)malloc(strlen(input)+1);
        strcpy(command,input);
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
        else if(strcmp(args[0],"jobs")==0)
        {
            jobs();
        }
        else if (strcmp(args[0],"fg")==0)
        {
            int num = atoi(args[1]);
            if(num<=0&&kill(psTable[num].pid,0)!=0)
            {
                printf("fg error");
                continue;
            }
            if(tcsetpgrp(STDIN_FILENO,getpgid(psTable[num].pid))!=0)
                printf("fg fore error");
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
                    {
                        *gloPid2 = pid2;
                        exit(87);
                    }
                    //showExecInfo();
                    FILE *trash;
                    if(background==true)
                    {
                        
                        //trash = fopen("/dev/null","r");
                        //fprintf(trash," ");
                        //dup2(fileno(trash),0);
                        //close(0);
                    }
                    if(setpgid(getpid(),getpid())!=0)
                        printf("setpgid error");
                    printf("2child pid %d pgid %d\n",getpid(),getpgid(0));

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
                    
                    printf("add [%d] %s\n",++psIndex,command);
                    psTable[psIndex].pid=*gloPid2;
                    psTable[psIndex].command=(char*)malloc(strlen(command)+1);
                    strcpy(psTable[psIndex].command,command);
                    free(command);
                }
                jobs();
                //checkJobs();                
            }
        }
        flushArgs();
    }
}

