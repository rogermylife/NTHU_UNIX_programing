#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#define DEFAULT 1024


pid_t shellPgid;
int shellTerminal;
int shellIsInteractive;
int jobIndex=0;
char *args[100];
int argc;
int background;

typedef struct job
{
    char *command;
    pid_t pgid;
    int pid;
    int completed;
    int stopped;
    int status;
}jobTable[DEFAULT];


void initShell()
{
    shellTerminal = STDIN_FILENO;
    shellIsInteractive = isatty(shellTerminal);

    if(shellIsInteractive)
    {
        printf("first fore gpid %d init pgid %d\n",tcgetpgrp(shellTerminal),getpgrp());
        while(tcgetpgrp(shellTerminal)!=(shellPgid=getpgrp()))
            kill(shellPgid,SIGTTIN);
        
        signal (SIGINT, SIG_IGN);
        signal (SIGQUIT, SIG_IGN);
        signal (SIGTSTP, SIG_IGN);
        signal (SIGTTIN, SIG_IGN);
        signal (SIGTTOU, SIG_IGN);
        signal (SIGCHLD, SIG_IGN); 

        printf("orin pgid %d pid %d\n",shellPgid,getpid());
        shellPgid = getpid();
        if(setpgid(shellPgid,shellPgid))
        {
            printf("could not put the shell in its own process group");
            exit (1);
        }

        tcsetpgrp(shellTerminal,shellPgid);

        //save default terminal attributes
    }
}

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
            background=1;
            pch = strtok(NULL," \n");
            continue;
        }
        if(pch[strlen(pch)-1]=='&')
        {
            background=1;
            pch[strlen(pch)-1]='\0';
        }
        args[argc] =(char*)malloc(strlen(pch)+1);
        strcpy(args[argc++],pch);
        pch = strtok(NULL," \n");
    }
    if(args[0]!=NULL&&args[argc-1][strlen(args[argc-1])]=='&')
        background=1;
    //strcpy(args[argc++],"0>/dev/null");
    //printf("check\n");
    //strcpy(args,input+strlen(command)+1);
    //printf("test %s %s\n",command,input+strlen(command)+1);
    args[argc]=NULL;
    //args[argc]=(char*)malloc(1);
    //args[argc][0]=0;
}

void jobs()
{
    printf("my jobs\n");
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

int main()
{
    initShell();
    char pwd[DEFAULT];
    char input[DEFAULT];
    jobIndex=0;
    while(1)
    {
        background = 0;
        argc=0;
        char *command;
        getcwd(pwd,DEFAULT);
        printf("%d %d $ ",getpid(),getpgid(0)); 
        fflush(stdout);
        fgets(input,DEFAULT,stdin);
        command = (char*)malloc(strlen(input)+1);
        strcpy(command,input);
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
        else if(strcmp(args[0],"exit")==0)
            exit(0);

        else if (strcmp(args[0],"fg")==0)
        {
            /*
            int num = atoi(args[1]);
            if(num<=0&&kill(psTable[num].pid,0)!=0)
            {
                printf("fg error");
                continue;
            }
            if(tcsetpgrp(STDIN_FILENO,getpgid(psTable[num].pid))!=0)
                printf("fg fore error");
            */
        }
        else
        {
          
        }
        flushArgs(); 
    }
}
