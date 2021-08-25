#include<stdio.h>
int main()
{  
    int n,x,m;
    scanf("%d%d",&x,&n);
    int i,j;
    i=n/7;
    j=n%7;
    m=5*250*i;
    int k,s=x;
    for(k=1;k<=j;k++){
        if(x!=6||x!=7)
        m+=250;
        x++;
    }
    printf("%d",m);
    return 0;
}
// 4/8