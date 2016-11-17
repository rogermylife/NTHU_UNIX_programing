#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int main(void){
 char line[1024];
 char *args[16];
 memset (args, 0, sizeof(args));
 printf("$ ");
 fflush(NULL); 
 fgets(line,sizeof(line),stdin);
 line[strlen(line)-1]='\0';
 args[0] = strtok(line, " ");
 args[1] = strtok(NULL, " "); 
 printf("%s\n" , args[0]);
 printf("%s", args[1]);
 fflush(NULL);
 execvp("ls", args);
 perror("execvp");
 return EXIT_FAILURE;
}
