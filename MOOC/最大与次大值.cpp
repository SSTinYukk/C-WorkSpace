#include <stdio.h>
int main()
{
    int n,a[100],i;
    scanf("%d",&n);
    for(i=0;i<n;i++)
    {
        scanf("%d,",&a[i]);
    }
    int j,temp;
    for(i=1;i<n;i++){
        temp=a[i];    
        for(j=i-1;j>=0;j--){
            if(temp>a[j]){
                a[j+1]=a[j];
            }else{
                break;
            }
        }
        a[j+1]=temp;
    }
    // for(i=0;i<n;i++){
    //     printf("%d",a[i]);
    // }
    printf("%d,%d",a[0],a[1]);
    return 0;
}
