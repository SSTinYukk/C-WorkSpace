#include<stdio.h>
int main(void){
    int n;
    scanf("%d",&n);
    int a[50][50]={0};
    int i,j,cnt=2;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            scanf("%d",&a[i][j]);
        }
    }
    
    int sum=0;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            if(i!=n-1-j&&i!=n-1&&j!=n-1){
            sum+=a[i][j];
        }
        }
    }
    printf("%d",sum);
}
  