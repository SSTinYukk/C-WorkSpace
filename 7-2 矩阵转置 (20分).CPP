#include<stdio.h>
int main(void){
    int n;
    scanf("%d",&n);
    int a[100][100];
    int i,j,cnt=1;
    for(j=0;j<n;j++){
        for(i=0;i<n;i++){
            a[i][j]=cnt;
            cnt++;
        }
    }
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            printf("%4d",a[i][j]);
        }
        printf("\n");
    }
}