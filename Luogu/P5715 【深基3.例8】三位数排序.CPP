#include <stdio.h>
int main(){
    int a,b,c;
    scanf("%d%d%d",&a,&b,&c);
    if(a>b){
        int t;
        t=a;
        a=b;
        b=t;
    }
    if(b>c){
        int t;
        t=c;
        c=b;
        b=t;
    }
      if(a>b){
        int t;
        t=a;
        a=b;
        b=t;
    }
    if(b>c){
        int t;
        t=c;
        c=b;
        b=t;
    }
    printf("%d %d %d",a,b,c);
    return 0;
}