#include<stdio.h>

int main(int argc, char *argv[]) {
  FILE *fp = fopen("Linux.txt", "wb");
  long long a = 0x78756e694c;
  fwrite(&a, sizeof(a), 1, fp);
  fclose(fp);
  return 0;
}