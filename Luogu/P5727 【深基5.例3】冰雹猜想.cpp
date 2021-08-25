#include<stdio.h>
int main()
{
    int a[1000],n,x,i=0;
    scanf("%d",&x);
    a[i]=x;
    while(x!=1){
        i++;
        if(x%2){
            x=x*3+1;
        }else{
            x/=2;
        }
        a[i]=x;
    }
    for(;i>=0;i--){
        printf("%d ",a[i]);
    }
}