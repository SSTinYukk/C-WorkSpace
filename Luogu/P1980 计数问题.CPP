#include<stdio.h>
int main()
{
    int n,x;
    scanf("%d%d",&n,&x);
    int i,cnt=0;
    for(i=1;i<=n;i++){
        int p=i,k;
        k=p%10;
        if(k==x){
            cnt++;
        }
        while (p/10>0){
                p/=10;
                k=p%10;
                if(k==x){
                    cnt++;
                }
            }
    }
    printf("%d",cnt);
    return 0;
}