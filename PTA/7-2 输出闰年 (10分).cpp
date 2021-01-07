#include<stdio.h>
int isrun(int year)
{
    if(year%100!=0&&year%4==0){
        return 1;
    }else if(year%400==0){
        return 1;
    }else{
        return 0;
    }
}
int main()
{
    int year,n,i,is=1;
    scanf("%d",&year);
    if(year>2100||year<2000){
        printf("Invalid year!");
        return 0;
    }
    for(i=2001;i<=year;i++){
        if(isrun(i)){
            printf("%d\n",i);
            is=0;
        }
    }
    if(is){
        printf("None");
    }
    return 0;
}