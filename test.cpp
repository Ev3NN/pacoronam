#include <iostream>

using namespace std;

class B
{
public:
    virtual ~B()
    {
        cout<<"B destructor"<<endl;
    }
};


class D : public B
{
public:
    virtual ~D()
    {
        cout<<"D destructor"<<endl;
    }
};

int main() {
    B *pD = new D();
    delete pD;
}