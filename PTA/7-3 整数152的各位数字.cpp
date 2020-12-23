#include<stdio.h>
int main(){
    int i,n,a[100];
    scanf("%d",&n);
    for(i=0;n>0;i++){
        a[i]=n%10;
        n/=10;
    }
    printf("%d = %d + %d*10 + %d*100",a[0]+a[1]*10+a[2]*100,a[0],a[1],a[2]);
    return 0;
}