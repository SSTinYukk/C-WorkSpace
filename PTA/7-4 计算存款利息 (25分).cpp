#include<stdio.h>
#include<math.h>
int main(){
    double interest,money,rate;
    double year;
    scanf("%lf %lf %lf",&money,&year,&rate);
    interest=money*pow((1+rate),year)-money;
    printf("interest = %.2f",interest);
}