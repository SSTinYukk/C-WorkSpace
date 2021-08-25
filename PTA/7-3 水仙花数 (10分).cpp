#include<stdio.h>
#include<math.h>
int main(){
    int i,j,n;
    scanf("%d",&n);
    for(i=pow(10,n-1);i<pow(10,n);i++){
        int sum=0,x=i,t;
        while(x>0){
            t=x%10;
            sum+=pow(t,n);
            x/=10;
        }
        if(!(sum-i)){
            printf("%d\n",i);
        }
    }
    return 0;
}