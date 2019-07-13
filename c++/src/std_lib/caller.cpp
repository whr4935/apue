#include <c_plus_plus_primer.h>
#include <functional>
#include <algorithm>

template <typename F, typename... Args>
auto makeCaller(F&& f, Args&&... args) -> std::function<decltype(f(args...))()>
{
    std::cout << "do makeCaller" << __FUNCTION__ << std::endl;
    return std::bind(std::forward<F>(f), std::forward<Args>(args)...);
}

//template <typename F, typename C, typename... Args>
//auto makeCaller(F&& f, C&& c, Args&&... args) -> std::function<void()>
//{
    //return std::bind(std::forward<F>(f), c, std::forward<Args>(args)...);
//}

void test1(int& i, int j)
{
    std::cout << "test1 i addr: " << &i << std::endl;
    std::cout << "i = " << i << " , j = " << j << std::endl;

    i = 88;
}

int test2()
{
    std::cout << "I am test2" << std::endl;
    return 10;
}

class Foo
{
public:
    //void print() {
        //std::cout << this << " Foo::print" << std::endl;
    //}

    void print(int a) {
        std::cout << this << " Foo::print: " << a << std::endl;
    }

    Foo() =default;
    Foo(const Foo&)=delete;
};

template <typename F, typename C, typename... ARGS>
class Action 
{
public:
    Action(F&& f, const std::weak_ptr<C>& c, ARGS&&...args)
    : mWeak(c)
    , mCallable(std::bind(std::forward<F&&>(f), std::placeholders::_1, std::forward<ARGS&&>(args)...)) 
    {

    }

    void operator()() const 
    {
        std::shared_ptr<C> mStrong = mWeak.lock();
        std::cout << "ptr strong: " << mStrong << std::endl;
        if (mStrong) {
            mCallable(mStrong.get());
        }
    }

private:
    std::weak_ptr<C> mWeak;
    std::function<void(C*)> mCallable;
};


class Processer
{
public:
    //template <typename F,  typename... ARGS>
        //bool push_task(F&& f, ARGS&&...args) {
            //auto func = std::bind(std::forward<F&&>(f), std::forward<ARGS&&>(args)...);

            //mActions.push_back(func);
        //}

    template <typename F, typename C, typename... ARGS>
    bool pushTask(const Action<F,C,ARGS...>& act) {
        mActions.push_back(act); 
    }

    void run() {
        for (auto &p : mActions) {
            p();
        }
    }

private:
    std::list<std::function<void(void)>> mActions;

};

template <typename F, typename C, typename... ARGS>
Action<F,C,ARGS...> makeAction(F&& f, std::weak_ptr<C>&& c, ARGS&&... args)
{
    return Action<F,C,ARGS...>(std::forward<F&&>(f), c, std::forward<ARGS>(args)...);
}

int test_caller()
{
    int a = 3;
    std::cout << "a's addr: " << &a << std::endl;
    auto f = makeCaller(test1, std::ref(a), 4);
    f();
    std::cout << "after f(), a = " << a << std::endl;
    auto f2 = makeCaller(test2);
    auto ret = f2();
    std::cout << "ret = " << ret << std::endl;

    Foo foo;
    std::cout << &foo << std::endl;
    //auto f3 = makeCaller(&Foo::print, foo);
    //auto f3 = makeCaller(&Foo::print, std::ref(foo));
    //f3();

    std::cout << "processer..." << std::endl;
    Processer processer;
    //processer.push_task(&Foo::print, &foo);
    //processer.pushTask(Action<F,C,ARGS...>(&Foo::print, &foo, 123));

    auto f1 = std::make_shared<Foo>();
    processer.pushTask(makeAction(&Foo::print, std::weak_ptr<Foo>(f1), 123));
    processer.run();
    std::cout << "processer...finished\n" << std::endl;

    return 0;
}
