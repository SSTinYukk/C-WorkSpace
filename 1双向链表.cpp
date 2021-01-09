#include<stdio.h>
#include<stdlib.h>

typedef struct node
{
    int data;/* data */
    int index;
    struct node *next,*former;
}Node;

typedef struct list
{
    Node *head;
    Node *tail;
}List;

void tailin(List *pList,int number)
{
    Node* p=(Node*)malloc(sizeof(Node));
    p->data=number;
    if(pList->head==NULL&&pList->tail==NULL){
        pList->tail=p;
        pList->head=p;
        p->former=pList->head;
        p->next=pList->tail;
    int index;
    }else{
        p->former=pList->tail;
        pList->tail->next=p;
        pList->tail=p;
        p->next=pList->tail;
    }
}

void headin(List *pList,int number)
{
    Node *p=(Node*)malloc(sizeof(Node));
    p->data=number;
    if(pList->head==NULL&&pList->tail==NULL){
        pList->head=p;
        pList->tail=p;
        p->former=pList->head;
        p->next=pList->tail;
    }else{
        p->next=pList->head;
        pList->head->former=p;
        pList->head=p;
        p->former=pList->head;
    }
}

void headprint(List *pList)
{
    Node* p;
    for(p=pList->head;p!=pList->tail;p=p->next){
        printf("%d ",p->data);
    }
    printf("%d\n",pList->tail->data);
}

void tailprint(List *pList)
{
    Node *p;
    for(p=pList->tail;p!=pList->head;p=p->former){
        printf("%d",p->data);
    }
    printf("%d\n",pList->head->data);
}

void headout(List *pList)
{
    Node *p=pList->head;
    pList->head=p->next;
    p->next->former=pList->head;
    free(p);
}
// void clean(List *pList)
// {
//     Node *p;
//     Node *q;
//     for(p=pList->head;p->next;){
//         q=p;
//         p=p->next;
//         free(q);
//     }
// }
void tailout(List *pList)
{
    Node *p=pList->tail;
    pList->tail=p->former;
    p->former->next=pList->tail;
    free(p);
}

int index(List *pList,int Index)
{
    Node *p;
    int dex=0;
    for(p=pList->head;p;p=p->next){
        if(Index==dex#include <stdio.h>
#include <stdlib.h>
 
typedef int ElemType;//声明定义数据类型为整型 
 
typedef struct Node//定义节点 
{
	ElemType data;
	struct Node *next;
} Node;
typedef struct Node *LinkList;//定义LinkList 
 
void CreateListHead(LinkList *L);
void PrintList(LinkList *L);
 
int main(int argc, char *argv[]) {
	LinkList list1;//声明一个链表list1 
	CreateListHead(&list1);//生成链表（头插法） 
	PrintList(&list1);//打印链表 
	return 0;
}
 
/*创建链表符合题目的链表，带头结点，使用头插法*/
void CreateListHead(LinkList *L)
{
	LinkList p;
	int number;
	*L=(LinkList)malloc(sizeof(Node));//生成头节点 
	(*L)->next=NULL;
	do
	{
		scanf("%d",&number);
		if(number!=-1)
		{
			p=(LinkList)malloc(sizeof(Node));//生成一个新的节点 
			p->data=number;
			p->next=(*L)->next;//将*L的后继赋值给p的后继 
			(*L)->next=p;//将p赋值给*L的后继 
		}	
	}while(number!=-1);//当number等于-1退出循环 
}
 
void PrintList(LinkList *L)
{
	LinkList p;
	for(p=(*L)->next;p->next!=NULL;p=p->next)//循环遍历打印最后一个节点之前的元素，因为链表list1有头结点所以p的初始值为(*L)->next，既第一元素节点 
	{
		printf("%d ",p->data);
	}
	printf("%d\n",p->data);//因为要求打印最后一个元素后不带空格，所以单独打印 
}){
            printf("%d\n",p->data);
            break;
        }
        dex++;
    }
    return p->data;
}
int main()
{   int number;
    scanf("%d")
    while()
}