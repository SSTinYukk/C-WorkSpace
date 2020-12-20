#include<stdio.h>
int main()
{
    int a,b,c;
    char s[4];
    scanf("%d%d%d",&a,&b,&c);
    scanf("%s",s);
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
    int i;
    for(i=0;i<3;i++){
        if(s[i]=='B'){
            printf("%d ",b);
        }
        if(s[i]=='C'){
            printf("%d ",c);
        }
          if(s[i]=='A'){
            printf("%d ",a);
        }
    }
}