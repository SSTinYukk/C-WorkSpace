#include<stdio.h>
int main()
{
    double f1;
    int i1;
    double f2;
    char c1;
    scanf("%lf %d %c %lf",&f1,&i1,&c1,&f2);
    printf("%c %d %.2f %.2f",c1,i1,f1,f2);
    return 0;
}