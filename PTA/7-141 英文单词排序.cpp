#include<stdio.h>
#include<string.h>
int main(){
    char s[100][100];
    int a[100]={0};
    int is[100]={0};
    int i=0,n=0;
    scanf("%s",&s[i++]);
    a[i-1]=strlen(s[i-1]);
    is[i-1]=strlen(s[i-1]);
    while(s[i-1][0]!='#'){
        scanf("%s",&s[i++]);
        a[i-1]=strlen(s[i-1]);
        is[i-1]=strlen(s[i-1]);
    }
    n=i-1;
    int j=0;
    for(i=0;i<n-1;i++){
        for(j=0;j<n-i-1;j++){
            if(a[j]>a[j+1]){
                int temp;
                temp=a[j];
                a[j]=a[j+1];
                a[j+1]=temp;
            }
        }
    }
    int k,index;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            if(a[i]==is[j]){
                index=j;
                printf("%s ",s[index]);
                is[j]=0;
                break;
            }
        }
    }
}