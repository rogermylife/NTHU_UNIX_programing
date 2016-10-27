#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(int argc,char **argv)
{
    if(argc!=3)
        fprintf(stderr,"usage: lcp <source file> <dest file>");

    int fin,fout;

    if((fin=open(argv[1], O_RDONLY, 0)) < 0)
        fprintf(stderr,"can not open file :%s",argv[1]);
    if((fout=open(argv[2],O_WRONLY|O_CREAT|O_EXCL,S_IRUSR|S_IWUSR))<0)
        fprintf(stderr,"can not create file :%s",argv[2]);

    char buf;
    int n;
    int holes=0;
    while(read(fin,&buf,1)>0)
    {
        if(buf!=0&&holes>0)
        {
            lseek(fout,holes,SEEK_CUR);
            write(fout,&buf,1);
            holes=0;
        }
        else if(buf==0)
        {
            ++holes;
            continue;
        }
        else
        {
            write(fout,&buf,1);
        }

    }

}
