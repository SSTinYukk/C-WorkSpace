#include <stdio.h>
int main(int argc, char *argv[]) 
{ 
    int val = 2018; 
    int *pi = 2019; 
    pi = &val; 
    *pi = 0; 
    return 0; 
}