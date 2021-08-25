#include<iostream>
using namespace std;
class AI{
    int i;
    double love;
    public:
    void set_i(int a){
        i=a;
    }
    int get_i(){
        return i;
    }
};

int main()
{
    AI a;
    a.set_i(12);
    cout <<a.get_i()<<endl;
    return 0;
}