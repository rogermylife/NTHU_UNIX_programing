#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(void)
{
	pid_t pid;
	
    if ((pid = fork()) < 0) {
        printf("fork error");
    } else if (pid == 0) {
        /*WAIT_PARENT();  parent goes first */
        int gid=setsid();
        if(gid==getpid())
            printf("child: uid=>%d gid=>%d  EQUAL And my session is %d \n",getpid(),gid,getsid(0));
    } else {
        /*charatatime("output from parent\n");*/
        waitpid(pid,NULL,0);
        printf("parent: child pid= %d my session id is %d \n",pid,getsid(0));
    }
	return(0);
}
