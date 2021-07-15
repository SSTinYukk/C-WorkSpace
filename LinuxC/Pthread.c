#include<stdio.h>
#include<pthread.h>

void childthread(void)
{
    int i;
    for(i=0;i<10;i++)
    {
        printf("CHild Pthread Success\n");
    }
    
}

int main