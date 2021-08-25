#include<stdio.h>

int main()
{
    char msg[256]={0};
    int num=0;
    char ch;
    do{
       ch=fgetc(stdin);
        msg[num++]=ch;
    }while(ch!='\n');
    msg[num]='\0';
}