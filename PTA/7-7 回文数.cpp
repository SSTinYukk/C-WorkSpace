#include<stdio.h>
int huiwen(int n)
{
    int x=n;
    int t=0;
    while(n>0){
        t*=10;
        t+=n%10;
        n/=10;
    }
    if(t==x){
        return 1;
    }else
    {
        return 0;
    }
}
int main(){
    int n;
    scanf("%d",&n);
    if(huiwen(n)){
        printf("yes");
    }else{
        printf("no");
    }
    return 0;
}