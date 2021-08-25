#include<iostream>
using namespace std;

int a=0;

void recv(void *a){}
void send(void *a){}

int main()
{
    int b=1;
    void (*p)(void *a)=&recv;
    cout<<"a ="<<&a<<endl;
    cout<<"b ="<<&b<<endl;
    cout<<"recv = "<<&recv<<endl;
    cout << "send = " << p << endl;
    printf("%p",p);
    return 0;
}