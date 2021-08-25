#include<stdio.h>
static int a = 2018;
static void func(void) {
  static int b;
  printf("a = %d, b = %d\n", a++, ++b);
}
int main(int argc, char *argv[]) {
  func();
  func();
  func();
  return 0;
}