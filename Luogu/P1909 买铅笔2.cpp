#include<stdio.h>
int main()
{
    int a,b,n,c=2147483647,i,j;
    scanf("%d",&n);
    for(i=0;i<3;i++){
        scanf("%d %d",&a,&b);
        for(j=1;j*a<n;j++);
        if(c>j*b){
            c=j*b;
        }
    }
printf("%d",c);
}