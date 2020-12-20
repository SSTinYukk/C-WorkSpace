#include<stdio.h>
#include<math.h>
int huiwen(int x)
{
    int temp,result;
    int i=x;
    temp=x%10;
    result=temp;
    x/=10;
    while(x>0){
        temp=x%10;
        result=result*10+temp;
        x/=10;
    }
    if(i-result){
        return false;
    }else{
        return true;
    }
}
bool isprime(int x){
    int i;
    for(i=2;i<=sqrt(x);i++){
        if(x%i==0){
            return false;
        }
    }
    return true;
}

int main(){
    int a,b,i;
    scanf("%d%d",&a,&b);
    if(!(a%2)){
        a--;
    }
    for(i=a;i<=b;i+=2){
        if((huiwen(i))&&isprime(i)){
            printf("%d\n",i);
        }
    }
    return 0;
}