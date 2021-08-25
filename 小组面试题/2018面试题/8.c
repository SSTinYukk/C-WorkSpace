#include<stdio.h>
int f(unsigned int num) {
    unsigned int i = 0;
  for (; num; i++) {
    printf("%u\n",num);
    num &= (num - 1);

  }
  return i;
}
int main()
{
    printf("%u",f(2018));
}
/*

2018
11111100010
11111100001
11111011111
11111100000
11111000



*/