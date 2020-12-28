#include<stdio.h>
#include<math.h>
typedef struct _Basic{
    int id;
    char name[10];
    int score;
}Basic;
int main(){
    int n,i=0;
    Basic Data[100];
    scanf("%d",&n);
    double ave=0;
    for(i=0;i<n;i++){
        scanf("%d%s%d",&Data[i].id,&Data[i].name,&Data[i].score);
        ave+=Data[i].score;
    }
    ave/=n;
    printf("%.2f\n",ave);
    for(i=0;i<=n;i++){
        if(Data[i].score-ave<0.0000001){
            printf("%s %d\n",Data[i].name,Data[i].score);
        }
    }
}