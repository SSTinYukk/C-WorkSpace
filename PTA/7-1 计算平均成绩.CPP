#include<stdio.h>
#include<math.h>
typedef struct _Basic{
    char id[10];
    char name[11];
    int score;
}Basic;
void swap(Basic *x,Basic *y){
    Basic t;
    t=*x;
    *x=*y;
    *y=t;
}
int main(){
    int n,i=0,j;
    Basic Data[100];
    scanf("%d",&n);
    double ave=0;
    for(i=0;i<n;i++){
        scanf("%s%s%d",&Data[i].id,&Data[i].name,&Data[i].score);
        ave+=Data[i].score;
    }
    ave/=n;
    for(i=0;i<n-1;i++){
        for(j=0;j<n-i-1;j++){
            if(Data[j].score>Data[j+1].score){
                swap(&Data[j],&Data[j+1]);
            }
        }
    }
    printf("%.2f",ave);
    for(i=0;i<=n;i++){
        if(Data[i].score<ave){
            printf("\n%s %s",Data[i].name,Data[i].id);
        }
    }
}