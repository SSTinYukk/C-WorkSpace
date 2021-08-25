#include<iostream>

using namespace std;

int* func2(){
    int *p=new int(122);
    return p;
}

int *func(){
    int *p=new int[10];
    return p;
}

int main()
{
    int * p =func();
    int *q=p;
    p[1]=11231212;
    cout<<p[1]<<endl;
    cout << q[1] << endl;
    delete p;
    q[1]=110;
    cout<<q[1]<<endl;
    return 0;
    
}