#include<stdio.h>
int main(){
    double m,n,bmi;
    scanf("%lf%lf",&m,&n);
    bmi=m/(n*n);
    bmi=(bmi*10000+0.5)/10000;
    if(bmi<18.5){
        printf("Underweight",bmi);
    }else if(bmi<24){
        printf("Normal",bmi);
    }else{
        printf("%.4f\nOverweight",bmi);
    }
    return 0;
}