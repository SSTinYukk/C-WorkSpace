#include<stdio.h>
#include<unistd.h>
int main(int argc,char** argv)
{
__pid_t pid;
pid=fork();
if(pid==-1)
{
    perror("Fork Error");
    exit(1);
}else if (pid==0)
{
    printf("Rreturn value:%d\n",pid);
    printf("CHild Process");
    printf("My Pid:%d\n",getpid());

}else{
    printf("Rreturn value:%d\n",pid);
    printf("FAther Process");
    printf("My Pid:%d\n",getpid());
}
return 0;
}