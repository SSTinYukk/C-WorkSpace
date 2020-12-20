	#include<stdio.h>
	int main() {
	char a[20]={0};
		int i=0,n=0,q=0;
		scanf("%c",&a[0]);
		for(i=1; a[i-1]!='E'; q=i,i++) {
			scanf("%c",&a[i]);
		}
		int W=0,L=0;
		for(i=0; i<q; i++) {
			if(a[i]=='W'){
				W++;
			}else if(a[i]=='L'){
				L++;
			}
			if(W>=11||L>=11) {
				printf("%d:%d\n",W,L);
				W=0;
				L=0;
			}
		}
		printf("%d:%d\n",W,L);
		W=0;
		L=0;
		printf("\n");
		for(i=0; i<q; i++) {
			if(a[i]=='W'){
				W++;
			}else if(a[i]=='L'){
				L++;
			}
			if(W>=21||L>=21) {
				printf("%d:%d\n",W,L);
				W=0;
				L=0;
			}
		}
		printf("%d:%d",W,L);
		return 0;
	}