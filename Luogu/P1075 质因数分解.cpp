#include<stdio.h>
#include<math.h>
bool isprime(int n){
    if(n==2)
    {
        return true;
    }
    for(int i=2;i<=sqrt(n);i++){
        if(n%i==0){
            return false;
        }
    }
    return true;
}
int main(){
    int p,n,i,j;
    scanf("%d",&n);
    for(i=2;i<n;i++){
        if(n%i==0&&isprime(i)){
            printf("%d",n/i);
            break;
        }
    }
    return 0;
}