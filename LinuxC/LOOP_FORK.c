#include<pthread.h>
#include<stdio.h>
#include<unistd.h>

int main(int argc,int argv)
{  
    int i;
    pid_t pid;

    for(i=0;i<5;i++){
        if(fork()==0)
            break;
    }
    if(i==5){
        printf("im father");
    }else{
        printf("im %dth child\n",i);
    }
}