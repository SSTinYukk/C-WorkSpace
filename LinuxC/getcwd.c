#include<unistd.h> 
#include<stdio.h>
#define MAX_SIZE 255
int main()
{
    char path[MAX_SIZE];
    getcwd(path,sizeof(path));
    puts(path);
}