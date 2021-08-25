#include<stdio.h>
int main(){
    int n;
    double m;
    scanf("%d",&n);
    if(n<=150){
        m=0.4463*n;
    }else if(n<=400){
        m=150*0.4463+(n-150)*0.4663;
    }else{
        m=150*0.4463+400*0.4663+(n-550)*0.5663;
    }
    printf("%.1f",m);
}