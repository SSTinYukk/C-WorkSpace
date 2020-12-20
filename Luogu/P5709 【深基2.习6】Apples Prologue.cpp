#include<stdio.h>
int main(){
    double m,t,s;
    double a,b;
    scanf("%lf%lf%lf",&m,&t,&s);
    a=m-(m*(s/(m*t)));
    if(a<0)
    a=0;
    printf("%d\n",(int)a);
}