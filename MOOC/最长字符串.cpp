#include<stdio.h>
#include<string.h>
char s[100][100];
int a[100];
int main()
{
    int n,i;
    scanf("%d",&n);
    for(i=0;i<n;i++){
        scanf("%s",&s[i]);
        a[i]=strlen(s[i]);
    }
    int max=a[0],maxdex;
    for(i=n-1;i>=0;i--){
        if(a[i]>=max){
            max=a[i];
            maxdex=i;
        }
    }
    printf("The longest is :%s",s[maxdex]);
    return 0;
}