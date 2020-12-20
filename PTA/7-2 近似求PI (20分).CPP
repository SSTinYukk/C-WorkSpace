#include<stdio.h>
int jiechen(int n)
{
    if(n<2){
        return 1;
    }else{
        return n*jiechen(n-1);
    }
}
int main()
{
    int n,i,j;
    double PI,PI2,p,eps,san=1,sum;
    scanf("%le",&eps);
    for(i=1,j=1;PI-p<eps;i++,p=PI,j+=2){
        sum=jiechen(i);
        san*=j;
        PI2+=(sum/san); 
        PI=PI2*2.0;
       printf("%.5f\n",PI);
    }
    printf("%.5f",PI);
}