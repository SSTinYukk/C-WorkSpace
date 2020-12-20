#include<stdio.h>
double fn(double n){
    if(n<=2){
        return 1;
    }else{
        return fn(n-1)+fn(n-2);
    }
}
int main(){
    double n,x;
    scanf("%lf",&n);
    x=fn(n);
    printf("%.2f",x);
    return 0;
}