#include<stdio.h>
int main()
{
    int n,a[100];
    scanf("%d",&n);
    int i;
    for(i=n-1;i>=0;i--){
        scanf("%d,",&a[i]);
    }
    for(i=0;i<n;i++){
        printf("%d",a[i]);
        if(i!=n-1){
            printf(",");
        }
    }
    return 0;
}