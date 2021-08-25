#include<stdio.h>
#include<string.h>

#define REG "/reg"
#define LOG "/log"
#define ADF "/add"
#define MSG "/m"


int main()
{
    char argv[256][256];
    int argc=0;
    char ch;
    int i=0,j=0;
    ch=getchar();
    while(ch!='\n'){
        if(ch==' '){
            argv[i][j]='\0';
            i++,j=0;
            ch = getchar();
        }
        argv[i][j] = ch;
        ch = getchar();
        j++;
    }
    int argc=i;
    for(i=0;i<=argc;i++){
        if(strcmp(REG,argv[i])){
            
        }else if(strcmp(LOG,argv[i])){

        }else if(strcmp(MSG,argv[i])){
            
        }
    }
}