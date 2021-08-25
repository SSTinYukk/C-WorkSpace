#include<stdio.h>
int a[100];
int main()
{
    int n,income=0,save,i;
    for(i=1;i<=12;i++){
        scanf("%d",&a[i]);
    }
    for(i=1;i<=12;i++){
        income+=300;
        if(income<a[i]){
            income=0;
            printf("-%d",i);
            return 0;
        }
        while(income-a[i]>=100){
            income-=100;
            save+=100;
        }
        income-=a[i];
    }
    save=save+save*0.2+income;
    printf("%d",save);
    return 0;
}