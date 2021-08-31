#include <stdio.h>
#define X a + b
int main(int argc, char *argv[]) {
  int a = 1, b = 1;
  printf("%d\t%d\n", X-X,X * X);
  return 0;
}

// 1+1-1+1=2
//1+1*1+1=3