#include<stdio.h>
int swap(int *x,int *y)
{
	int t;
	t=*x;
	*x=*y;
	*y=t;
}

int bubSort(int a[],int n)
{
	int i,j,t;
	for(i=0;i<n-1;i++){
		for(j=0;j<n-i-1;j++){
			if(a[j]>a[j+1])
				swap(&a[j],&a[j+1]);
		}
	for(int c=0;c<n;c++)
	{
		printf("%d ",a[c]);
	}
	printf("\n");
	}
}

int selectSort(int a[],int n)
{
	int i,j,min;
	for(i=0;i<n;i++){
		for(j=i;j<n;j++){
			min=i;
			if(a[j]<a[min])
				min=j;
		}
		swap(&a[i],&a[min]);
		for(int c=0;c<n;c++)
	{
		printf("%d ",a[c]);
	}
	printf("\n");
	}
 } 

int InsertSort(int a[],int n)
{
	int i,j,t;
	for(i=1;i<n;i++)
	{
		t=a[i];
		for(j=i-1;j>=0&&a[j]>t;j--)
		{
			a[j+1]=a[j];
		}
		a[j+1]=t; 
	for(int c=0;c<n;c++)
	{
		printf("%d ",a[c]);
	}
	printf("\n");
	}
}

void QuickSort(int a[],int l,int r)
{
	if(l>=r)
		return;
	int i,j,piv;
	i=l;
	j=r;
	piv=a[l];
	
/*	while()
	{
		while(a[j]>piv&&i>l) j--;
		while(a[i]<piv&&i<r) i++;
		if(i<j){
			swap(&a[i],&a[j]);
		}
		swap(&a[l],&a[i]);
	}								*/
	while(true)
	{
		while(a[++i]<piv&&i<r);
		while(a[--j]>piv&&j>l);
		
		if(i>j) 
			break;
		swap(&a[i],&a[j]);
	}
	swap(&a[l],&a[j]);
	QuickSort(a,l,j-1);
	QuickSort(a,j+1,r);
}
void QMarkSort(int a[],int l,int r)
{
	int mark,piv,i;
	if(l>=r) return;
	mark=l;
	piv=a[l];
	
	for(i=l;i<=r;i++){
		if(a[i]<piv){
			
			mark++;
			swap(&a[mark],&a[i]);
		}
	}
	
	swap(&a[mark],&a[l]);
	QMarkSort(a,l,mark-1);
	QMarkSort(a,mark+1,r);
}
int main()
{
	int n=9,i,j;
	int a[10]={14,7,23,31,40,56,78,9,2};
	//QMarkSort(a,0,n);
	//QuickSort(a,0,n);
	InsertSort(a,n);
	//bubSort(a,n);
	//selectSort(a,n);
	for(i=0;i<n;i++)
	{
		printf("%d ",a[i]);
	}
	return 0;	
}
