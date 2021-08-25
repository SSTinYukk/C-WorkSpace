#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
int i = 1;
void exx(int signal){
    printf("%d",i);
    return;
}
int main(void){
    signal(SIGALRM, exx);
    alarm(1);
    while(i++){
        //fprintf(stdout,"%d\n",i++);
        //printf("%d\n", i++);
    }
    // for(;;i++){
    //     fprintf(stdout,"%d\n",i++);
    //     //printf("%d\n", i++);
    // }
}