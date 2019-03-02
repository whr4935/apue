#include <iostream>

namespace A
{
    using namespace std;

    void f() {
        cout << "abc" << endl;
    }
}

int main(int argc, char *argv[])
{
    A::f(); 
    return 0;
}
