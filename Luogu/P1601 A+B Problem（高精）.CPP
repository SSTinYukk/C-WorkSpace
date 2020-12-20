#include<stdio.h>
#include<string.h>
char b[1000000]={0},a[10000000]={0},s[1000000],sa[1000000],sb[10000000]={0};
int main()
{
    
    int i;
    scanf("%s%s",a,b);
    int len;
    if(strlen(a)>strlen(b)){
        len=strlen(a);
    }else{
        len=strlen(b);
    }
    int j;
    for(i=strlen(a)-1,j=0;i>=0;j++,i--){
        sa[i]=a[j]-48;
    }
    for(i=strlen(b)-1,j=0;i>=0;j++,i--){
        sb[i]=b[j]-48;
    }
    for(i=0;i<len;i++){
        if(sa[i]+sb[i]>9){
            s[i]+=sa[i]+sb[i]-10;
            s[i+1]++;
        }else{
            s[i]+=sa[i]+sb[i];
        }
        if(s[i]>9){
            s[i]-=10;
            s[i+1]++;
        }
    }
    if(s[len]!=0)
        len++;
    for(i=len-1;i>=0;i--){
        printf("%d",s[i]);
    }
}