#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<errno.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>

#define MAX_SIZE 255

int main(int argc,char **argv)
{
        char buf[4096];
        char originalPath[MAX_SIZE];
        char targetPath[MAX_SIZE];
        int i=0;
        int inf,outf;
        inf=open(argv[1],O_RDONLY);
        outf=open(argv[2],O_RDWR|O_CREAT|O_TRUNC,777);
        do{
                i=read(inf,buf,4096);
                write(outf,buf,i);
        }while(i>0);
        close(inf);
        close(outf);
        return 0;
}