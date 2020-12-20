#include<stdio.h>
int main(){
    int a,b,c,d,e,f;
    scanf("%d%d%d%d",&a,&b,&c,&d);
    if(d<b){
        d=d+60;
        c--;
    }
    e=c-a;
    f=d-b;
    printf("%d %d",e,f);
}