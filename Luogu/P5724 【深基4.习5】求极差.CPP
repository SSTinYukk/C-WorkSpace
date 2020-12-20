#include<stdio.h>
int main()
{
    int a[1000],n,min,max;
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        scanf("%d",&a[i]);
    }
    max=min=a[0];
    for(int i=0;i<n;i++){
        if(min>a[i]){
            min=a[i];
        }
        if(max<a[i]){
            max=a[i];
        }
    }
    printf("%d",max-min);
    return 0;
}