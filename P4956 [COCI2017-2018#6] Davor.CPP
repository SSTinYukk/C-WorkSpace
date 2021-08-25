#include<stdio.h>
int main()
{
    int n,k,i;
    scanf("%d",&n);
    for(k=1;;k++){
        for(i=100;i>=1;i--){
            if((7*i+21*k)*52==n){
                printf("%d\n%d",i,k);
                return 0;
            }
        }
    }
}