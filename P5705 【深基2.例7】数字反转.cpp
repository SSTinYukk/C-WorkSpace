#include<stdio.h>
#include<string.h>
int main(){
	char a[10];
	scanf("%s",a);
	int i;
	for(i=strlen(a);i>=0;i--){
		if(a[i]>='0'&&a[i]<='9')
		printf("%c",a[i]);
		if(i==1)
		printf(".");
	}
    printf("\n");
}
