#include<stdio.h>
int main()
{
    int a[100],n,cnt=0,i;
    for(i=0;i<10;i++){
        scanf("%d",&a[i]);
    }
    scanf("%d",&n);
    for(i=0;i<10;i++){
        if(a[i]<=n+30)
        {
            cnt++;
        }
    }
    printf("%d",cnt);
    return 0;
}