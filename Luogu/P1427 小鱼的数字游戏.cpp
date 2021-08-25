#include<stdio.h>
int main()
{
    int a[100],i=0,j=0,n=0;
    scanf("%d",&a[i]);
    while(a[i]!=0){
    n++;
    i++;
    scanf("%d",&a[i]);
    }
    for(i=n-1;i>=0;i--){
        printf("%d ",a[i]);
    }
}