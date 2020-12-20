#include<stdio.h>
int main()
{
    double a,b;
    int cnt=0;
    scanf("%lf",&a);
    while(a>=1){
        a/=2;
        cnt++;
    }
    printf("%d",cnt);
}