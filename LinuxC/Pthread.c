#include<stdio.h>
#include<pthread.h>

void* childthread(void* x)
{
    int i;
    for(i=0;i<10;i++)
    {
        printf("CHild Pthread Success\n");
    }
    
}
int main(){
    pthread_t tid;
    printf("Create childthread\n");
    pthread_create(&tid,NULL, childthread,NULL);
    pthread_join(tid,NULL);
    printf("childthread exit");
    return 0;
}