#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void func(char a[]) 
{  
        printf( "%lu\n",sizeof(a));
        printf( "%lu\n",strlen(a));
}
int main( )
{
        char *b=(char *)malloc(sizeof(char)*10);
        char a[] = "hello world";                              
        printf("%p\t%p\t%p\n",a,&a,&a[0]);
        printf("%p\t%p\t%p\n",b,&b,b[0]);
}
