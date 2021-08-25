#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

int i = 1;

void* cude(void * arg)
{
    alarm(10);
    while (i++)
    {
        //fprintf(stdout,"%d\n",i++);
        //printf("%d\n", i++);
    }
    // for(;;i++){
    //     fprintf(stdout,"%d\n",i++);
    //     //printf("%d\n", i++);
    // }
}

int main(void)
{
    pthread_t tid;  
    pthread_create(&tid,0,cude,0);
    pthread_create(&tid, 0, cude, 0);
    pthread_create(&tid, 0, cude, 0);
    pthread_create(&tid, 0, cude, 0);
    pthread_create(&tid, 0, cude, 0);
    pthread_create(&tid, 0, cude, 0);
    sleep(10);
    printf("%d\n",i);
    pthread_exit(0);
}