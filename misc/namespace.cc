//直接using是不行的，必须看到声明
//using std::cout;

#include <iostream>
#include <vector>

typedef std::vector<int> aacout;

//using naemspace abc;

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
public:
    struct Inner {
        Inner(Base* b)
        {
            this->b = b;
        }

        void test() {
            printf("mName:%d\n", b->mName);
        }

        Base* b;
    };

private:
    int mName = 89;
};

int main(int argc, char *argv[])
{
    Base b;

    Base::Inner inner(&b);
    inner.test();


    const Base& rb = b;
    const Base& b2 = rb;

    std::cout << "a" <<std::endl;

    printf("b2:%p, rb:%p", &b2, &rb);


    A::cout << "abc" << A::endl;

    A::f(); 

    Base base2;
    //int mm = b.mName;

    return 0;
}
