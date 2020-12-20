#include<stdio.h>
int is(int x)
{
    if(!(x%4)&&x%100){
        return 1;
    }else if(x%400==0)
    {
        return 1;
    }else{
        return 0;
    }
    
}
int main(){
    int a[13]={0,31,29,31,30,31,30,31,31,30,31,30,31};
    int b[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
    int m,n;
    scanf("%d%d",&n,&m);
    if(is(n)){
        printf("%d",a[m]);
    }else{
        printf("%d",b[m]);
    }
    return 0;
}