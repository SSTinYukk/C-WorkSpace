#include "stdio.h"
#include <stdlib.h>

union strings 
{
    char a[20];
    int s[5];
    int *p;
};

struct uu
{
    int a;
    union strings x;
    int b;
};

int main()
{
    struct uu a;
    printf("%p\t%p\t%p\t%p",&a.x.a,&a.x.a[1],&a.x.a[2],&a.x.a[3]);
}