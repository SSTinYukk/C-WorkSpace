#include<stdio.h>
int main()
{
    int n,s;
    scanf("%d",&n);
    if(n==0){
        s=0;
    }else if(n<0){
        s=-1;
    }else{
        s=1;
    }
    printf("sign(%d) = %d",n,s);
    return 0;
}