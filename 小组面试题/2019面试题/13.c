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
    int b;
    union string x;
};

int main()
{
}