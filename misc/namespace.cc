//直接using是不行的，必须看到声明
//using std::cout;

#include <iostream>
#include <vector>

typedef std::vector<int> aacout;

//using namespace std;
namespace A
{
    using namespace std;

    void f() {
        cout << "abc" << endl;
    }
}


class Base
{

};

int main(int argc, char *argv[])
{
    Base b;

    const Base& rb = b;
    const Base& b2 = rb;

    cout << "a" <<std::endl;

    printf("b2:%p, rb:%p", &b2, &rb);


    A::cout << "abc" << A::endl;

    A::f(); 
    return 0;
}
