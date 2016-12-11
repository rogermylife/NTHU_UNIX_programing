#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>


pid_t shellPgid;
int shellTerminal;
int shellIsInteractive;

void initShell()
{
    shellTerminal = STDIN_FILENO;
    shellIsInteractive = isatty(shellTerminal);

    if(shellIsInteractive)
    {
        //printf("first fore gpid %d init pgid %d\n",tcgetpgrp(shellTerminal),getpgrp());
        while(tcgetpgrp(shellTerminal)!=(shellPgid=getpgrp()))
            kill(shellPgid,SIGTTIN);
        
        signal (SIGINT, SIG_IGN);
        signal (SIGQUIT, SIG_IGN);
        signal (SIGTSTP, SIG_IGN);
        signal (SIGTTIN, SIG_IGN);
        signal (SIGTTOU, SIG_IGN);
        signal (SIGCHLD, SIG_IGN); 

        //printf("orin pgid %d pid %d\n",shellPgid,getpid());
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

int main()
{
    initShell();
}
