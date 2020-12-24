#include<stdio.h>
int main()
{
    double  ave,min=214748,max,mark;
    int n,i;
    scanf("%d",&n);
    for(i=0;i<n;i++){
        scanf("%lf",&mark);
        ave+=mark/n;
        if(mark>max){
            max=mark;
        }
        if(mark<min){
            min=mark;
        }
    }
    printf("average = %.2f\n",ave);
    printf("max = %.2f\n",max);
    printf("min = %.2f\n",min);
    return 0;
}