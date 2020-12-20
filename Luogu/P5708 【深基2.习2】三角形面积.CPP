#include<stdio.h>
#include<math.h>
int main()
{
    double a,b,c;
    double s,p;
    scanf("%lf%lf%lf",&a,&b,&c);
    p=1.0/2.0*(a+b+c);
    s=sqrt(p*(p-a)*(p-b)*(p-c));
    printf("%.1f",s);
    return 0;
}
//(int)(s*10+0.5)/10.0)