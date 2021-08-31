#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
  char *p = (char *)malloc(sizeof(char) * 20);
  char *q = p;

  scanf("%s %s", p, q);
  char *p2 = (char *)malloc(sizeof(char) * 20);
  scanf("%s",p2);
  printf("%s %s\n", p, q);
  free(p);
  return 0;
}