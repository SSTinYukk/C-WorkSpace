#include<stdio.h>
int main()
{
    int a,b,c;
    scanf("%d%d%d",&a,&b,&c);
    if(a>b){
        int t ;
        t=a;
        a=b;
        b=t;
    }
    if(b>c){
        int t ;
        t=b;
        b=c;
        c=t;
    }
     if(a>b){
        int t ;
        t=a;
        a=b;
        b=t;
    }
    if(b>c){
        int t ;
        t=b;
        b=c;
        c=t;
    }
    int t;
    int m=a,n=c;
    while(n>0){
        t=m%n;
        m=n;
        n=t;
    }
    printf("%d/%d",a/m,c/m);
    return 0;
}