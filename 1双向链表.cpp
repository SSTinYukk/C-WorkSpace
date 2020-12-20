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
        if(Index==dex){
            printf("%d\n",p->data);
            break;
        }
        dex++;
    }
    return p->data;
}

int main(void){
    int number;
    List list;
    list.head=NULL;
    list.tail=NULL;
    scanf("%d",&number);
    while (number!=-1)
    {
        tailin(&list,number);
        scanf("%d",&number);
    }
    headout(&list);
    tailout(&list);
    headprint(&list);
    index(&list,1);
    return 0;
}