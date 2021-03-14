#include <stdio.h>
typedef struct node
{
    char id[17];
    int test;
    int stand;
} Node;
Node js[10000];
int a[10000] = {0};
int main()
{
    int i, n;
    scanf("%d", &n);
    for (i = 0; i < n; i++)
    {
        scanf("%s%d%d", js[i].id, &js[i].test, &js[i].stand);
    }
    int j, x;
    scanf("%d", &x);
    for (i = 0; i < x; i++)
    {
        scanf("%d", &a[i]);
    }
    for (j = 0; j < x; j++)
    {
        for (i = 0; i < n; i++)
        {
            if (a[j] == js[i].test)
            {
                printf("%s %d\n", js[i].id, js[i].stand);
            }
        }
    }
}