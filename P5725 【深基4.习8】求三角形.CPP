#include<stdio.h>
int main()
{
    int n,i,j,cnt=1;
    scanf("%d",&n);
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            printf("%02d",cnt++);
        }
        printf("\n");
    }
    printf("\n");
    cnt=1;
    for(i=1;i<=n;i++){
        int k=n-i;
        for(;k>0;k--){
            printf("  ");
        }
        k=i;
        for(;k>0;k--)
        {
            printf("%02d",cnt++);
        }
        printf("\n");
    }
}