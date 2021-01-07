#include<stdio.h>
    long long int i,j,n,x=2,y=1,z=3;
    double sum=0;
int main()
{

    scanf("%ld",&n);
    for(i=0;i<n;i++){
        sum+=(double)x/y;
        y=x;
        x=z;
        z=x+y;
    }
    printf("%.2f",sum);
    return 0;
}