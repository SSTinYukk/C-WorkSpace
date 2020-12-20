#include<stdio.h>
#include<math.h>
bool isPrime(int n)
{
    int i;
    for(i=2;i<=sqrt(n);i++){
        if(n%i==0){
            return false;
        }
    }
    return true;
}
int main(){
    int n,l,i,k,cnt=0,sum=0;
    scanf("%d",&k);
    for(i=2;;i++){
        int is=0;
        is=isPrime(i);
        if(is){
            sum+=i;
            cnt++;
            if(sum>k){
                sum-=i;
                cnt--;
                break;
            }
            printf("%d\n",i);
        }
    }
    printf("%d",cnt);
}