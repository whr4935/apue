#include <c_plus_plus_primer.h>
#include<type_traits>
#include "template.h"
#include "Blob.h"
#include <string.h>

//////////////////////////////////////////////////////////////////////////
//函数声明
static int test_template_func();
static int test_blob();

//////////////////////////////////////////////////////////////////////////
int TEST_template(int argc, char **argv)
{
    int ret = 0;

    ret = test_template_func();
    // ret = test_blob();

    return ret;
}

template<class T, unsigned N>
int compare(T  *v1, T  *v2)
{
    *v1 = *v2;

    // std::cout << N << endl;

    /* std::less */

    if (*v1 > *v2) return 1;
    if (*v1 < *v2) return -1;
    return 0;
}

// template<unsigned N, unsigned M>
// int compare(const char (&p1)[N], const char (&p2)[M])
// {
    // // cout << "N = " << N << " M = " << M << endl;
    // return strcmp(p1, p2); 
// }

static int cmp(int a, int b)
{
    if (a > b)
        return 1;
    else if (a < b)
        return -1;
    return 0;
}

template <typename T>
int compare(const T& v1, const T& v2)
{
    if (v1 < v2) return -1;
    if (v2 < v1) return 1;
    return 0;
}

template<>
int compare(const char* const &p1, const char* const &p2)
{
    std::cout << "call specialization version" << std::endl;
    return strcmp(p1, p2);
}

//////////////////
template<typename T, typename F = decltype(cmp)>
int compare_v2(const T& v1, const T& v2, F f = &cmp)
{
    if (f(v1, v2)) return -1;
    if (f(v2, v1)) return -1;
    return 0;
}

template<typename It>
int fcn(It beg, It end)
{
    return *beg;
}

template<typename T>
T fr_ref(T&& v)
{
    std::cout << v << std::endl;
    return v;
}

// template<typename T>
// int f_variadic(const T& t)
// {

// }

/////////////////////////////////////////
template<typename T, typename... Args>
int f_variadic(const T& t, const Args&... rest)
{
    cout << "sizeof...(Args):" << sizeof...(Args) << ", sizeof...(rest):" << sizeof...(rest) << endl;

    T a = "gk";
    std::cout << a[1] << std::endl;

    return 0;
}

static int test_template_func()
{
    int v1 = 1;
    int v2 = 2;

    // std::cout << "compare 1 and 2, result: " << compare<int, 8>(&v1, &v2) << std::endl;

    std::cout << "compare_array:" << compare("hello", "world") << std::endl;

    const char* p1 = "hello", *p2 = "world";
    std::cout << "compare pointer:" << compare(p1, p2) << std::endl;

    compare_v2(3, 4);

    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    fcn(v.begin(), v.end());

    fr_ref(10);
    fr_ref(v1);

    /////////////////
    std::cout << " <<<<< test variadic template >>>" << std::endl;
    f_variadic("aa");
    f_variadic("aa", 1, 2, 3);

    return 0;
}

static int test_blob()
{
    Blob<int>::tr = 10;

    Blob<int> b;
    Blob<int> ia2 = {1, 2, 3, 4};
    int a1[] = {1, 2, 3, 4, 5};

    DebugDelete de;
    char *p = new char[10];
    de(p);
    
    std::unique_ptr<int, DebugDelete> up(new int, DebugDelete());
    std::unique_ptr<string, DebugDelete> sp(new std::string, DebugDelete());

    Blob<int> b3(std::begin(a1), std::end(a1));

    Number<int, true> n(3);


    Foo<int> fi;
    fi.Bar();

    decltype((42)) i;
    decltype(b) bb;
    // decltype((b)) bb2;

}


int main(int argc, char *argv[])
{
    
    return 0;
}
