#include <c_plus_plus_primer.h>
#include "expression.h"

static int test_expression_op();

//////////////////////////////////////////////////////////////////////////
int TEST_expression(int argc, char **argv)
{
    std::cout << "======================"<< std::endl;
    std::cout << __FUNCTION__ << "\n" << std::endl;
    int ret;

    ret = test_expression_op();

    std::cout << "======================\n"<< std::endl;
    return ret;
}

static int test_expression_op()
{
    int r;

    double a {3.14};
    int b{a};

    // m % (-n) is equal to m % n, and(-m) % n is equal to -(m % n)
    //也就是说，取模的正负号由denominator m决定

    r = 21 % 5;		//1
    r = -21 % 5;		//-1
    r = 21 % -5;		//1
    r = -21 % -5;	//-1

    std::cout << "-21%5: " << -21 % 5 << std::endl;
    std::cout << "-21/5: " << -21 / 5 << std::endl;
    std::cout << "-21%-5: " << -21 % -5 << std::endl;
    std::cout << "-21/-5: " << -21 / -5 << std::endl;
    std::cout << "21%-5: " << 21 % -5 << std::endl;
    std::cout << "21/-5: " << 21 / -5 << std::endl;

    switch (int i = 3) {
    case 2:

        break;
    case 3:
        cout << "i = " << i << endl;
        break;
    }

    int m_do = 3;
    do {
        m_do = 0;

    } while (m_do == 3);	//do while条件中的变量不能是定义在do语句块中的变量

#if 0
    string s1, s2;
    while (cin >> s1 >> s2) {
        try {
            if (s1 == s2)
                cout << s1 << endl;
            else
                throw std::runtime_error("input no equal");
        } catch (std::runtime_error err) {
            cout << err.what() << endl;
            cout << "\nInput again? yes or no" << endl;
            string c;
            cin >> c;
            if (!cin || c[0] == 'n') {
                cout << "break..." << endl;
                break;
            }
        } catch (...) {
            cout << "catch except!" << endl;
        }
    }
#endif

    return 0;
}

//constexpr 是能在编译时确定值的常量表达式，而const是可能在运行时才能确定值
static constexpr int const_func(int i)
{
//	i = 3;			//constexpr函数只能是一个return语句
    return i * 3;
}
static int test_constexpr()
{
#if 0
    const int num = rand();
#else
    const int num = 100;
#endif

    constexpr int c = const_func(num);
    return 0;
}
