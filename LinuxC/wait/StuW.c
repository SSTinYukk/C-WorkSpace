#include<stdio.h>
#include<unistd.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/wait.h>


typedef struct student
{
    int id;
    char name[BUFSIZ];
    char sign[BUFSIZ];
    
}student;
void sys_err(char *str)
{
    perror(str);
    exit(1);
}

int main(int argc,char** argv)
{
    int ret;
    int fd;
    pid_t pid;
    student stu={1,"LiMing","DO FOR IT"};
    student *p;
    
    fd=open("a.txt",O_RDWR|O_CREAT|O_TRUNC,0664);
    ftruncate(fd,sizeof(stu));
    if(ret==-1)
        sys_err("Open");
    p=(student *)mmap(NULL,sizeof(stu),PROT_WRITE|PROT_READ,MAP_SHARED,fd,NULL);
    if(p==MAP_FAILED)
        sys_err;
    pid=fork();
    if(pid>0){
        memcpy(p,&stu,sizeof(stu));
        sleep(1);
    }else if(pid==0){
        while(1)
        printf("ID:%d   NAME:%s\nSignature:%s\n",p->id,p->name,p->sign);
    }
}
