#include<stdio.h>
int main(void)
{
    int n,i,j,sum=1,x=3,c,s,t=1;
    char p;
    scanf("%d %c",&n,&p);
    while(sum<n){
        c=sum;
        sum+=x*2;
        x+=2;
        t++;
    }
    if(n==1){
        printf("%c\n0",p);  
        return 0;    
    }
    s=n-c;
    t=t-1;
    for(i=t;i>0;i--){
        for(j=i;j<t;j++){
            printf(" ");
        }
        for(j=i*2-1;j>0;j--){
            printf("%c",p);
        }
        printf("\n");
    }
    for(i=2;i<=t;i++){
        for(j=i;j<t;j++){
            printf(" ");
        }
        for(j=i*2-1;j>0;j--){
            printf("%c",p);
        }
        printf("\n");
    }
    printf("%d",s);
    return 0;
}