#include<stdio.h>
int main()
{
    int a[10000];
    int n,time,i;
    scanf("%d%d",&n,&time);
    for(i=0;i<time;i++){
        int l,r;
        scanf("%d%d",&l,&r);
        for(;l<=r;l++){
            a[l]=1;
        }
    }
    int num;
    for(i=0;i<=n;i++){
        if(a[i]){
            continue;
        }else{
            num++;
        }
    }
    printf("%d",num);
}