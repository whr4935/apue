#include <iostream>

//using namespace std;
namespace A
{
    using namespace std;

    void f() {
        cout << "abc" << endl;
    }
}

int main(int argc, char *argv[])
{
    A::cout << "abc" << A::endl;

    A::f(); 
    return 0;
}
