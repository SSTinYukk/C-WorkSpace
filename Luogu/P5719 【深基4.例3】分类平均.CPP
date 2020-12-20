#include<stdio.h>
int main()
{
    int n,k;
    double ave1=0,ave2=0,sum1=0,sum2=0;
    scanf("%d%d",&n,&k);
    int i;
    for(i=1;i<=n;i++)
    {
        if(i%k==0){
            sum1+=i;
            ave1++;
        }else{
            sum2+=i;
            ave2++;
        }
    }
    ave1=sum1/ave1;
    ave2=sum2/ave2;
    printf("%.1f %.1f",ave1,ave2);
}