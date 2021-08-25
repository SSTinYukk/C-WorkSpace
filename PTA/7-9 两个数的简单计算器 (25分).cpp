#include<stdio.h>
int main(){
    char c;
    int a,sum;
    scanf("%d",&sum);
    scanf("%c",&c);
    scanf("%d",&a);
    while(c!='='){
        switch (c)
        {
        case '+':
            sum+=a;
            break;
        case '-':
            sum-=a;
            break;
        case '*':
            sum*=a;
            break;
        case '/':
            sum/=a;
            break;
        case '%':
            sum%=a;
            break;

        
        default:
            break;
        }
    scanf("%c",&c);
    if(c=='='){
        goto loop;
    }
    scanf("%d",&a);
    }
    loop:printf("%d",sum);
}