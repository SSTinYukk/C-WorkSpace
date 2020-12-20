#include<stdio.h>
int main(){
    int n,temp,x,i,f=1;
    scanf("%d",&n);
    if(n<0){
        n=-1*n;
        f=-1;
    }
    temp=n%10;
    x=temp;
    n/=10;
    while(n>0){
        temp=n%10;
        x=x*10+temp;
        n/=10;
    }
    printf("%d",f*x);
    return 0;
}