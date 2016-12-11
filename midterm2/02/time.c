#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(void)
{
	time_t t;
	struct tm *tmp;
	char buf1[16];
	char buf2[500];
	time(&t);
	tmp = localtime(&t);
	strftime(buf2, 500, "%T, %A %B %e, %G", tmp);
    printf("%s\n", buf2);
	exit(0);
}
