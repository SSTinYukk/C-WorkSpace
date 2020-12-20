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
        if(s==7){
            m-=250;
        }
        if(s==6){
            m-=250;
        }
        if(s!=6||s!=7||s!=0)
        m+=250;
         if(s==7){
            s=0;
        }
        s++;
    }
    printf("%d",m);
    return 0;
}
// 4/8