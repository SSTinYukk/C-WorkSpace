#include<stdio.h>
int jie(int n)
{
    if(n<2){
        return 1;
    }else{
        return n*jie(n-1);
    }
}
int main(){
    int n,x;
    scanf("%d",&n); 
    x=jie(n);
    printf("%d",x);
    return 0;
}