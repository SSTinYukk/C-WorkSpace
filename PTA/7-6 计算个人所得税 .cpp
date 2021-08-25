#include<stdio.h>
int main()
{
    double tax,money;
    scanf("%lf",&money);
    if(money<=1600){
        tax=0;
    }else if(money<=2500){
        money-=1600;
        tax=money*0.05;
    }else if(money<=3500){
        money-=1600;
        tax=money*0.1;
    }else if(money<=4500){
        money-=1600;
        tax = money*0.15;
    }else{
        money-=1600;
        tax = money * 0.2;
    }
    printf("%.2f",tax);
    return 0;
}