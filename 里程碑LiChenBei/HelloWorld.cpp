#include<stdio.h>
#include<string.h>
int main(){
    char n[]="Hello World!!\b\n";
    printf("%s",n);
    char a[]="Welcome";
    strcpy(n,a);
    main();
    return 0;
}