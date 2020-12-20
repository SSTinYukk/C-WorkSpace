#include<stdio.h>
int main(){
    char a[10];
    int num;
    char c;
    scanf("%c-%c%c%c-%c%c%c%c%c-%c",&a[0],&a[1],&a[2],&a[3],&a[4],&a[5],&a[6],&a[7],&a[8],&a[9]);
    if(a[9]=='X')
    a[9]='9'+1;
    num=(a[0]-48)*1+(a[1]-48)*2+(a[2]-48)*3+(a[3]-48)*4+(a[4]-48)*5+(a[5]-48)*6+(a[6]-48)*7+(a[7]-48)*8+(a[8]-48)*9;
    c=num%11;
    if(a[9]-48==c){
        printf("Right\n");
    }else{
        c=c+48;
        if(c==58){
            c='X';
        }
        printf("%c-%c%c%c-%c%c%c%c%c-%c\n",a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8],c);
    }
    return 0;
}