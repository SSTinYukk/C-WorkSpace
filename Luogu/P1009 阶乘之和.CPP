#include<stdio.h>
int jie(int n)
{
    if(n<2){
        return 1;
    }else{
        return jie(n)*jie(n-1);
    }
}
int main()
{
    int m,n;
}