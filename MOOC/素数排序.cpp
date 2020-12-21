#include<stdio.h>
#include<math.h>
bool isprime(int n)
{
    int i;
    for(i=2;i<=sqrt(n);i++){
        if(n%i==0){
            return false;
        }
    }
    return true;
}
int main()
{
    int n=10,is=0,a[100],i;
    for(i=0;i<10;i++){
        scanf("%d",a[i]);
    }
    for(i=0;i<10;i++)
    {
        if(!isprime(a[i])){
            a[i]=0;
            n--;    
        }
    }
    int j;
    for(i=0;i<9;i++){
        for(j=0;i<9-i;j++){
            if(a[j]>a[j+1]){
                int t=a[j];
                a[j]=a[j+1];
                a[j+1]=t;
            }
        }
    }
    for(i=0;i<n;i++){
        printf("%d",a[i]);
    }
    return 0;
}