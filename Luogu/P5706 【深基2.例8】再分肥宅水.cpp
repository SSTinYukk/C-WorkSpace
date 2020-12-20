#include<stdio.h>
int main(void){
    double t;
    int n;
    scanf("%lf%d",&t,&n);
    printf("%.3f\n%d",t/n,n*2);
    return 0;
}