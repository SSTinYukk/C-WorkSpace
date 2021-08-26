#include <stdio.h>
void f() 
{ 
 static int a = 0; 
 int b = 0; 
 printf("%d, %d\n", ++a, ++b); 
return 0; 
}
int main()
{
    
}