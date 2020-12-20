#include<stdio.h>
void swap(int* x,int* y)
{
    int t;
    t=*x;
    *x=*y;
    *y=t;
}
int main(){
    int a[10000]={0},s[10000]={0};
    int i=0,cnt=0,n;
    scanf("%d",&n);
    for(i=0;i<n;i++){
        scanf("%d",&a[i]);
    }
    int j=0;
    for(i=0;i<n;i++){
        while(a[i]+1==a[i+1]){
            s[j]++;
            i++;
        }
        j++;
    }
    int k=j,min;
    // for(i=0;i<=k;i++){
    //     min=s[i];
    //     int x;
    //     for(j=i+1;j<=k;j++){
    //         if(s[j]<min){
    //             min=s[j];
    //             x=j;
    //         }
    //     }
    //     swap(&s[i],&s[min]);
    // }
    // for(i=0;i<k-1;i++){
    //     for(j=0;j<k-i-1;j++){
    //         if(s[j]>s[j+1]);
    //         swap(&s[j],&s[j+1]);
    //     }
    // }
    int max=0;
    for(i=0;i<=k;i++){
        if(s[i]>max)
        {
            max=s[i];
        }
    }
    // for(i=0;i<=k;i++){
    //     printf("%4d",s[i]);
    // }
    printf("%d",max+1);
    return 0;
}