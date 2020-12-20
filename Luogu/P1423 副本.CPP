#include<cstdio>
#include<iostream>
using namespace std;
int main()
{
    double a,b=0,c=2,d=0;
    cin>>a;
    for(;b<a;d++,b+=c,c*=0.98);
    cout<<d;
    return 0;
}