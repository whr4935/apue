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

int main(int argc, char *argv[])
{
    A::cout << "abc" << A::endl;

    A::f(); 
    return 0;
}
