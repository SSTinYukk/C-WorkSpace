#include<stdio.h>
int main()
{
    int m,n,i,a[100]={0};
    scanf("%d,%d",&n,&m);
    for(i=0;i<n-1;i++){
        scanf("%d,",&a[i]);
    }
    scanf("%d",&a[n-1]);
    int j=0,temp;
    for(j=0;j<m;j++){
        temp=a[n-1];
        for(i=n-1;i>0;i--){
            a[i]=a[i-1];
        }
        a[0]=temp;
    }
    for(i=0;i<n;i++){
        printf("%d",a[i]);
        if(i!=n-1){
            printf(",");
        }
    }
    return 0;
}