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
int main()
{
    int x;
    scanf("%d",&x);
    printf("%d",is(x));
    return 0;
}