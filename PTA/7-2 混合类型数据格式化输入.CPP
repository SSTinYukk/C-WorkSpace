#include<stdio.h>
int main()
{
    double f1;
    int i1;
    double f2;
    char c1;
    scanf("%lf%i%c%lf",&f1,&i1,&f2,&c1);
    printf("%c %i %.2f %.2f",c1,i1,f1,f2);
    return 0;
}