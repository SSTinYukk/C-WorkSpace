#include<stdio.h>
int main()
{
    int k,n=0,coin;
    scanf("%d",&k);
    while(k>0){
        int i;
        for(i=1;i<=n+1;i++){
            coin+=n+1;
            k--;
            if(k==0){
                break;
            }
        }
        n++;
    }
    printf("%d",coin);
    return 0;
}