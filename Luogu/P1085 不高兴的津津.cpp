#include<stdio.h>
int main()
{
    int a[10],b[10];
    int m,n,i,j;
    for(i=1;i<8;i++){
        scanf("%d%d",&a[i],&b[i]);
    }
    int max=0;
    for(i=7;i>0;i--){
        if(a[i]+b[i]>8){
            j=i;
            if(a[i]+b[i]>=max){
                j=i;
            }
        }
    }
    printf("%d",j);
    return 0;
}