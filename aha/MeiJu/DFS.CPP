#include <cstdio>
using namespace std;
int a[100][100];//地图数组    1表示空地 0表示障碍物
int v[100][100]; //0表示未访问  1表示已访问
int p,q,min=99999999;//pq为终点坐标，Min为最小路径长度
void dfs(int x,int y,int step)
{
    if(x==p &&y ==q){
        if(step<min)
            min=step;
        return;
    }
    //顺时针试探
    if(a[x][y+1]==1&&v[x][y+1]==0)
    {
        v[x][y+1]=1;
        dfs(x,y+1,step+1);
        v[x][y+1]=0;
    }

    if(a[x+1][y]==1&&v[x+1][y]==0)
    {
        v[x+1][y]=1;
        dfs(x+1,y,step+1);
        v[x+1][y]=0;
    }

      if(a[x][y-1]==1&&v[x][y-1]==0)
    {
        v[x][y-1]=1;
        dfs(x,y-1,step+1);
        v[x][y-1]=0;
    }

        if(a[x-1][y]==1&&v[x-1][y]==0)
    {
        v[x-1][y]=1;
        dfs(x-1,y,step+1);
        v[x-1][y]=0;
    }
}