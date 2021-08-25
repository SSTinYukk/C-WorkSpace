#include<stdio.h>
void swap(int *x,int *y)
{
    int t;
    t=*y;
    *y=*x;
    *x=t;
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
        while(a[--j]>piv&&j>l);
		while(a[++i]<piv&&i<r);
		
		if(i>=j) 
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
// void QuickSort(int a[],int l,int r)
// {
//     if(l>=r){
//         return;
//     }
//     int piv,left,right;
//     left=l;
//     right=r;
//     piv=a[l];
//     while(true)
//     {
//         while(a[left++]<piv&&left<r);
//         while(a[right--]>piv&&right>l);
//         if(left>right)
//         {
//             break;
//         }
//         swap(&a[left],&a[right]);
//     }
//     swap(&a[l],&a[right]);

//     QuickSort(a,l,right-1);
//     QuickSort(a,right+1,r);
// }
int a[10000]={0};
int main()
{
    int n,i;
    scanf("%d",&n);
    for(i=0;i<n;i++){
        scanf("%d",&a[i]);
    }
    QMarkSort(a,0,n-1);
    double sum=0,ave=0;
    for(i=1;i<n-1;i++){
        sum+=a[i];
    }
    ave=sum/(n-2);
    printf("%.2f",ave);
    return 0;
}