#include<stdio.h>
int main()
{
    char a[11]={'0','1','2','3','4','5','6','7','8','9','-'};
    char b[11][10]={"ling","yi","er","san","si","wu","liu","qi","ba","jiu","fu"};
    char n[100];
    scanf("%s",n);
    int i=0;
    while(n[i]!=0){
        if(n[i+1]!=0){
            if(n[i]=='0')
                printf("%s ",b[0]);
            if(n[i]=='1')
                printf("%s ",b[1]);
            if(n[i]=='2')
                printf("%s ",b[2]);
            if(n[i]=='3')
                printf("%s ",b[3]);
            if(n[i]=='4')
                printf("%s ",b[4]);
            if(n[i]=='5')
                printf("%s ",b[5]);
            if(n[i]=='6')
                printf("%s ",b[6]);
            if(n[i]=='7')
                printf("%s ",b[7]);
            if(n[i]=='8')
                printf("%s ",b[8]);
            if(n[i]=='9')
                printf("%s ",b[9]);
            if(n[i]=='-')
                printf("%s ",b[10]);
        }else{
            if(n[i]=='0')
                printf("%s",b[0]);
            if(n[i]=='1')
                printf("%s",b[1]);
            if(n[i]=='2')
                printf("%s",b[2]);
            if(n[i]=='3')
                printf("%s",b[3]);
            if(n[i]=='4')
                printf("%s",b[4]);
            if(n[i]=='5')
                printf("%s",b[5]);
            if(n[i]=='6')
                printf("%s",b[6]);
            if(n[i]=='7')
                printf("%s",b[7]);
            if(n[i]=='8')
                printf("%s",b[8]);
            if(n[i]=='9')
                printf("%s",b[9]);
            if(n[i]=='-')
                printf("%s",b[10]);
        }
        i++;
    }
    return 0;

}