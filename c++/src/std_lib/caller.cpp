#include <c_plus_plus_primer.h>
#include <functional>

template <typename F, typename... Args>
auto makeCaller(F&& f, Args&&... args) -> std::function<decltype(f(args...))()>
{
    return std::bind(std::forward<F>(f), std::forward<Args>(args)...);
}

template <typename F, typename C, typename... Args>
auto makeCaller(F&& f, C&& c, Args&&... args) -> std::function<void()>
{
    return std::bind(std::forward<F>(f), c, std::forward<Args>(args)...);
}

void test1(int i, int j)
{
    std::cout << "i = " << i << " , j = " << j << std::endl;
}

int test2()
{
    std::cout << "I am test2" << std::endl;
    return 10;
}

class Foo
{
public:
    void print() {
        std::cout << this << " Foo::print" << std::endl;
    }

    Foo() =default;
    //Foo(const Foo&)=delete;
};

int test_caller()
{
    auto f = makeCaller(test1, 3, 4);
    f();
    auto f2 = makeCaller(test2);
    auto ret = f2();
    std::cout << "ret = " << ret << std::endl;

    Foo foo;
    std::cout << &foo << std::endl;
    //auto f3 = makeCaller(&Foo::print, foo);
    auto f3 = makeCaller(&Foo::print, std::ref(foo));
    f3();

    return 0;
}
