#include <stdio.h>

void dectobin( int n );

int main()
{
    int n;

    scanf("%d", &n);
    dectobin(n);

    return 0;
}

/* 你的代码将被嵌在这里 */
void dectobin(int n)
{
    int a[64]={0};
    int i=0;
    while(n>0){
        a[i++]=n%2;
        n/=2;
    }
    i--;
    for(;i>=0;i--){
        printf("%d",a[i]);
    }
}