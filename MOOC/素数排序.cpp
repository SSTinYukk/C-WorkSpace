#include<stdio.h>
#include<math.h>
int isprime(int n)
{
    
    int i;
    if(n==1)
    return 0;
    if(n==2){
        return 1;
    }
    for(i=2;i<=sqrt(n);i++){
        if(n%i==0){
            return 0;
        }
    }
    return 1;
}
int main()
{
    int a[1000000001];
    int n=10,is=0,i;
    for(i=0;i<n;i++){
        scanf("%d,",&a[i]);
    }
    int t=0;
    for(i=0;i<n;i++)
    {
        if(!isprime(a[i])){
            a[i]=0;
            t++;
        }else{
            is=1;
        }
    }
    
    int j;
    for(i=0;i<9;i++){
        for(j=0;j<9-i;j++){
            if(a[j]>a[j+1]){
                int t=a[j];
                a[j]=a[j+1];
                a[j+1]=t;
            }
        }
    }
    // for(i=0;i<n;i++){
    //     printf("%d ",a[i]);
    // }
    // printf("\n");

        for(i=t;i<n;i++){
            printf("%d",a[i]);
            if(i!=n-1){
                printf(",");
            }
        }
        if(is==0){
            printf("Not found");
        }
    return 0;
}