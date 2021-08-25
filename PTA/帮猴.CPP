#include<stdio.h>
#define N 10
struct boysscore{
	int minites;
	int seconds;
};
struct girlscore{
	char flexibility;
	int number;
};
struct student{
	char num[9];
	char name[21];
	char sex;
	union  
	{
		struct boysscore bs;
		struct girlscore gs;
	}score;
};
int main()
{
		struct student stu[N];
		int n,i;
		scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		scanf("%s %s %c",stu[i].num,stu[i].name,&stu[i].sex);
		if(stu[i].sex=='b'||stu[i].sex=='B')
			scanf("%d %d",&stu[i].score.bs.minites,&stu[i].score.bs.seconds);
		else if(stu[i].sex=='g'||stu[i].sex=='G')
			scanf(" %c %d",&stu[i].score.gs.flexibility,&stu[i].score.gs.number);
		else
		printf("input error!"); 
	}
	printf("\nthe information is:\n");
	for(i=0;i<n;i++)
	{
		printf("%-8s%s\t",stu[i].num,stu[i].name,stu[i].sex);
		if(stu[i].sex=='g'||stu[i].sex=='G')
		printf("%c and %d\n",stu[i].score.gs.flexibility,stu[i].score.gs.number);
		else if(stu[i].sex=='b'||stu[i].sex=='B')
		printf("%d:%d\n",stu[i].score.bs.minites,stu[i].score.bs.seconds); 
	}
}