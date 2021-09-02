#include<stdio.h>
int main()
{
    int a;
    char c;
    while(scanf("%d",&a)!=1)
    {
        while((c=getchar())!='\n'&&c!=EOF);
    };
    printf("%d\n",a);
}