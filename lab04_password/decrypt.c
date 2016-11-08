#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<crypt.h>
#include<omp.h>
#include<string.h>
int main(int argc,char *argv[])
{
    const char * words[] = {
            "password",
            "computer",
            "tigger",
            "summer",
            "internet",
            "service",
            "canada",
            "hello",
            "ranger",
            "shadow",
            "baseball",
            "donald",
            "harley",
            "hockey",
            "letmein",
            "maggie",
            "bare",
            "mustang",
            "snoopy",
            "buster",
            "dragon",
            "nthu",
            "michael",
            "michelle",
            "we",
            "mindy",
            "patrick",
            "bears",
            "andrew",
            "ppap",
            "calvin",
            "changeme",
            "diamond"
    };
    
    char input[10000];
    //char *p = crypt("pawword","ab");
    //printf("%s\n",p);
    FILE *file = fopen(argv[1],"r");
    while(fgets(input,10000,file))
    {
        //printf("1  %s\n",crypt("password","ab"));
        //printf("2  %s\n",crypt("password","$1$abcdefgh$"));
        int iam = 0, np = 1;
        
        #pragma omp parallel default(shared) private(iam, np)
        {
            #if defined (_OPENMP)
                np = omp_get_num_threads();
                iam = omp_get_thread_num();
            #endif
                //printf("Hello from thread %d out of %d\n", iam, np);
        } 

        int method;
        char salt[100];
        char ans[5000];
        char option[1000];
        char temp[123];
        //puts(input);
        //sscanf(input,"%s$%d$%s$%s",temp,&method,salt,ans);
        //sscanf("XDDDD$123123123","%s$%s",ans,salt);
        char *pch=strtok(input,"$:");
        pch = strtok(NULL," $:");
        //printf("check %s\n",pch);
        strcpy(temp,pch);
        method = atoi(temp);
        pch = strtok(NULL," $:");
        strcpy(salt,pch);
        pch = strtok(NULL," $:");
        strcpy(ans,pch);
        //printf("salt %s %d %s\n",salt,method,ans);
        sprintf(option,"$%d$%s$",method,salt);
        //printf("XDD  %s\n",option);

        int len = strlen(option);
        int abort=0;

        //char *bear ="webarebears";
        #pragma omp parallel for
        for(int i=0;i<31;++i)
        {
            
            #pragma omp flush (abort)
            if(abort==0)
            {   
                for(int j=i+1;j<32&&abort==0;++j)
                {
                    for(int k=j+1;k<33&&abort==0;++k)
                    {
                        
                        char guess[6][100];
                        
                        //sprintf(guess[0],"%s%s%s",words[i],words[j],words[k]);
                        guess[0][0]='\0';
                        strcat(guess[0],words[i]);
                        strcat(guess[0],words[j]);
                        strcat(guess[0],words[k]);
                        //sprintf(guess[1],"%s%s%s",words[i],words[k],words[j]);
                        guess[1][0]='\0';
                        strcat(guess[1],words[i]);
                        strcat(guess[1],words[k]);
                        strcat(guess[1],words[j]);
                        //sprintf(guess[2],"%s%s%s",words[j],words[i],words[k]);
                        guess[2][0]='\0';
                        strcat(guess[2],words[j]);
                        strcat(guess[2],words[i]);
                        strcat(guess[2],words[k]);
                        //sprintf(guess[3],"%s%s%s",words[j],words[k],words[i]);
                        guess[3][0]='\0';
                        strcat(guess[3],words[j]);
                        strcat(guess[3],words[k]);
                        strcat(guess[3],words[i]);
                        //sprintf(guess[4],"%s%s%s",words[k],words[i],words[j]);
                        guess[4][0]='\0';
                        strcat(guess[4],words[k]);
                        strcat(guess[4],words[i]);
                        strcat(guess[4],words[j]);
                        //sprintf(guess[5],"%s%s%s",words[k],words[j],words[i]);
                        guess[5][0]='\0';
                        strcat(guess[5],words[k]);
                        strcat(guess[5],words[j]);
                        strcat(guess[5],words[i]);
                        //printf("pick %s\n",guess[0]);
                        for(int l=0;l<6;++l)
                        {
                            char *str = crypt(guess[l],option);
                            //printf("matching %s %s\n",str+len,ans);
                            if(strcmp(str+len,ans)==0)
                            {
                                printf("%s\n",guess[l]);
                                abort = 1;
                                #pragma omp flush (abort)
                            }
                            
                        }
                    }
                }
            }
        }
    }
}
