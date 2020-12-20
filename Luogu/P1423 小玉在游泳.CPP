#include<stdio.h>
int main()
{
    double len,x=2,sum=0;
    scanf("%lf",&len);
    int cnt;
    while(sum<len){
        sum+=x;
        x*=0.98;
        cnt++;
    }
    printf("%d\n",cnt);
    return 0;
}