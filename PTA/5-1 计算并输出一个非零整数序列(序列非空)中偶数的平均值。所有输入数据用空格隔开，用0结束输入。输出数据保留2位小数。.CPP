#include <stdio.h>
int main()
{
   int number;
   int sum,n;
   double average;
   n = 0;
   sum = 0;
   scanf("%d",&number);

   while(
number!=0
){
        if(number%2 == 0){
            sum += number;
            n ++;
        }
        
scanf("%d",&number);
;
   }
   
average=(double)sum/n;
;
   printf("%.2lf\n",average);
   return 0;
}