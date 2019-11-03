#include <c_plus_plus_primer.h>
#include "misc.h"
#include "Sales_item.h"

static int misc();
static int test_sale_item(int argc, char **argv);
static int test_dtor_sequence();
static int test_namespace();
extern int test_signleton();
extern int test_thread_local();

//////////////////////////////////////////////////////////////////////////
int TEST_misc(int argc, char **argv)
{
    std::cout << "======================"<< std::endl;
    std::cout << __FUNCTION__ << "\n" << std::endl;

    int ret = 0;

    //misc();

    // ret = test_sale_item(argc, argv);
    //ret = test_dtor_sequence();
    //ret = test_namespace();
    //test_signleton();
    test_thread_local();

    std::cout << "======================\n"<< std::endl;
    return ret;
}

static int misc()
{
    //有符号数和无符号数一起运算，有符号数会提升为无符号数
    //无符号负数回滚：加上对应的范围回滚为整数，比如char型, -1 + 256 = 255
    //声明的同时并赋值是定义。

    int i = -256;
    int n;
    n = i % 255;
    unsigned int num = 1;

    std::cout << -1 * num << std::endl;	//std::cout: 4294967295


    long double pi = 3.14;
//	int a{pi};							list初始化内建变量时，不允许向下类型转换。
//	int b(pi);

    int m1 = 0, **pp2 = nullptr;		//**是类型修饰符，类型是int

    int ci;
    const int &r = ci;
    const int &rcpi = pi;
    //int &rpi = pi;

//	char *p = __FUNC__;
    using SI = Sales_item;

    SI item;
    const long double *cpi = &pi;

    auto ppi = &pi;
    decltype(ppi) ppi2;
    decltype((cpi)) ppi3 = cpi;
    decltype(*ppi2) ppi4 = pi;

    auto ccpi = &pi;
    auto pccpi = &ccpi;
    auto &ccpi2 = pccpi;

    return 0;
}

static int test_sale_item(int argc, char **argv)
{
    Sales_item total, trans;

    if (std::cin >> total) {
        while (std::cin >> trans) {
            if (trans.isbn() == total.isbn()) {
                total += trans;
            } else {
                std::cout << total << std::endl;
                total = trans;
            }
        }
        std::cout << total << std::endl;
    } else {
        std::cout << "no data?" << std::endl;
        return -1;
    }
}

static int test_dtor_sequence()
{
    class A {
    public:
        A() {std::cout << "ctor A" << std::endl;}
        ~A() {std::cout << "dtor A" << std::endl;}
    };

    class B {
        A a;
    public:
        B() {std::cout << "ctor B" << std::endl;}

        ~B() {std::cout << "dtor B" << std::endl;}
    };

    B *pb = new B;
    std::cout << "test dtor..." << std::endl;
    delete pb;
    pb = nullptr;
}

namespace B
{
int fun()
{
    std::cout << "B::fun" << std::endl;
}
}  // namespace B

namespace A {
    int fun() {
        std::cout << "A::fun" << std::endl;
    }

    namespace B {
        int fun() {
            std::cout << "A::B::fun" << std::endl;
        }
    }

    void caller()
    {
        B::fun();
        std::vector<int> s;
        s.push_back(1);
        std::cout << s.size() << std::endl;
    }
}


static int test_namespace()
{
    //using namespace A;
    //fun();

    A::caller();

    return 0;
}
