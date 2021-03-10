#include<stdio.h>
int main()
{
	char a[1000];
	int b[10]={0};
	scanf("%s", a);
	int i=0,j;
	while (a[i] != 0) {
		for (j = 0; j<10;j++) {
			b[j]++;
		}
		i++;
	}
	for (i = 0; i < 10; i++) {
		if (b[i] != 0) {
			printf("%d:%d\n", i, b[i]);
		}
	}
}