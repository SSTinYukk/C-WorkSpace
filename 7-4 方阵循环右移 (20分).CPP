#include<stdio.h>
int main()
{
    int a[100][100]={0};
    int m,n,i,j;
    scanf("%d%d",&m,&n);
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            scanf("%d",&a[i][j]);
        }
    }
    int h,cnt=0;
    for(i=0;i<n;i++){
        for(j=0,h=m-1;j<n;j++){
            printf("%d ",a[i][h++]);
            if(h==n){
            h=0;
        }
        }
        cnt++;
        if(cnt<n){
        printf("\n");
        }
    }
}