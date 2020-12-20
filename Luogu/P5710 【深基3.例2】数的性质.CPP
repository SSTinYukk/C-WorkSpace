#include<stdio.h>
int is(int x)
{
    if(x%2){
        return 0;
    }else
    {
        return 1;
    }
}
int is2(int x)
{
    if(x>4&&x<=12){
        return 1;
    }else
    {
        return 0;
    }
    
}
int main(){
    int x,a,b,c,d;
    scanf("%d",&x);
    a=is(x)&&is2(x);
    b=is(x)||is2(x);
    c=is(x)&&!is2(x)||!is(x)&&is2(x);
    d=!is(x)&&!is2(x);
    printf("%d %d %d %d",a,b,c,d);
    return 0;
}