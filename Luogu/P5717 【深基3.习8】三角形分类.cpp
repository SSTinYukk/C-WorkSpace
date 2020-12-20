#include<stdio.h>
int main()
{
    int a,b,c;
    scanf("%d%d%d",&a,&b,&c);
      if(a>b){
        int t;
        t=a;
        a=b;
        b=t;
    }
    if(b>c){
        int t;
        t=c;
        c=b;
        b=t;
    }
      if(a>b){
        int t;
        t=a;
        a=b;
        b=t;
    }
    if(b>c){
        int t;
        t=c;
        c=b;
        b=t;
    }
    if(a+b>c){
        
        if(a*a+b*b>c*c){
                printf("Acute triangle\n");
                if(a==b||b==c){
            printf("Isosceles triangle\n");
            }
                if(a==b&&b==c){
                    printf("Equilateral triangle\n");
                }
            }else if(a*a+b*b==c*c){
                printf("Right triangle\n");
            }else{
                
                printf("Obtuse triangle\n");
                if(a==b||b==c){
            printf("Isosceles triangle\n");
            }
            }
        }else{
            printf("Not triangle\n");
        }
    return 0;
}