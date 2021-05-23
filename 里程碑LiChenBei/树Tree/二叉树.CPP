#include<stdio.h>
#include<stdlib.h>
#define ElementType int 
typedef struct TNode *Position;
typedef Position BinTree; /* 二叉树类型 */
struct TNode{
    ElementType Data;
    BinTree Left;
    BinTree Right;
};

void PreOrderTraversal(BinTree BT)/* 先序遍历 */
{
    if(BT){
        printf("%d",BT->Data);
        PreOrderTraversal(BT->Left);
        PreOrderTraversal(BT->Right);
    }
}

void InOrderTraversal(BinTree BT)
{
    if(BT){
        InOrderTraversal(BT->Left);
        printf("%d",BT->Data);
        InOrderTraversal(BT->Right);
    }
}

void PostOrderTraversal(BinTree BT)
{
    if(BT){
        PostOrderTraversal(BT->Left);
        PostOrderTraversal(BT->Right);
        printf("%d",BT->Data);
    }
}   

Position Find(ElementType X,BinTree BST)
{
    if(!BST) return NULL;
    if(X>BST->Data)
        return Find(X,BST->Right);
    else if(X<BST->Data)
        return Find(X,BST->Left);
    else
        return BST;
}

Position IterFind(ElementType X,BinTree BST)
{
    while(BST){
        if(X>BST->Data)
            BST=BST->Right;
        else if(X<BST->Data)
            BST=BST->Left;
        else
            return BST;/*查找成功，返回节点找到的地址*/
    }
    return NULL;/*查找失败*/
}

Position FindMin(BinTree BST)
{
    if(BST) 
        while (BST->Left) BST=BST->Left;
    else
        return NULL;
}

Position FindMax(BinTree BST)
{
    if(BST) return NULL;
    else if(!BST->Right)
        return BST;
    else 
    return FindMax(BST->Right);
}

BinTree Insert(BinTree BST,ElementType X)
{
    if(!BST){/* 若原树为空，生成并返回一个结点的二叉搜索树 */
        BST = (BinTree)malloc(sizeof(struct TNode));
        BST->Data= X;
        BST->Left=BST->Right= NULL;
    }
    else{/* 开始找要插入元素的位置 */
        if(X<BST->Data)
            BST->Left = Insert(BST->Left,X);
        else if(X>BST->Data)
            BST->Right=Insert(BST->Right,X);
    }
}

BinTree Delete(ElementType X,BinTree BST)
{
BinTree Delete( BinTree BST, ElementType X ) 
{ 
    Position Tmp; 

    if( !BST ) 
        printf("要删除的元素未找到"); 
    else {
        if( X < BST->Data ) 
            BST->Left = Delete( BST->Left, X );   /* 从左子树递归删除 */
        else if( X > BST->Data ) 
            BST->Right = Delete( BST->Right, X ); /* 从右子树递归删除 */
        else { /* BST就是要删除的结点 */
            /* 如果被删除结点有左右两个子结点 */ 
            if( BST->Left && BST->Right ) {
                /* 从右子树中找最小的元素填充删除结点 */
                Tmp = FindMin( BST->Right );
                BST->Data = Tmp->Data;
                /* 从右子树中删除最小元素 */
                BST->Right = Delete( BST->Right, BST->Data );
            }
            else { /* 被删除结点有一个或无子结点 */
                Tmp = BST; 
                if( !BST->Left )       /* 只有右孩子或无子结点 */
                    BST = BST->Right; 
                else                   /* 只有左孩子 */
                    BST = BST->Left;
                free( Tmp );
            }
        }
    }
    return BST;
}