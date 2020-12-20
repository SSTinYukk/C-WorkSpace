#include<stdio.h>   
int main()
{
    int a[60][60]={0};
    int h,l,i,j;
    scanf("%d%d",&h,&l);
    for(i=0;i<h;i++){
        for(j=0;j<l;j++){
            scanf("%d",&a[i][j]);
        }
    }
    int sum[10];
    for(i=0;i<h;i++){
        for(j=0;j<l;j++){
            sum[i]+=a[i][j];
        }
    }
    for(i=0;i<h;i++){
        printf("%d\n",sum[i]);
    }
    return 0;
}