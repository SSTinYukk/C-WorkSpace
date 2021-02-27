//树的双亲表示法结构定义
#define MAX_TREE_SIZE 100
//孩子节点
typedef struct CTNode
{
    int child;      //孩子节点的下标
    struct CTNode *next; 
}*ChildPtr;
//表头结构;
typedef struct
{
    char data;
    int parent;
    ChildPtr firstchild;
}CTBox;

//树结构;
typedef struct{
    CTBox nodes[MAX_TREE_SIZE]; //节点
}
