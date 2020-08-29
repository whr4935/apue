#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

#define TEST_VECTOR_ERASE

using namespace std;

int main(int argc, char *argv[])
{
#ifdef TEST_VECTOR_ERASE
    vector<int> v{1, 2, 3, 4, 5};
    vector<int>::iterator it  = v.begin() + 2;
    v.erase(it);
#else
    list<int> v{1, 2, 3, 4, 5};
#endif

#if 0
    for (auto it = v.begin(); it != v.end();) {
        if (*it % 2 == 0) {
            it = v.erase(it);
            //删除list元素时，可以用 erase(it++),但是vecotr不行
            //v.erase(it++);
        } else {
            ++it;
        }
    }
#endif

    v.erase(std::remove_if(v.begin(), v.end(), [](int v) {return v%2 == 0;}), v.end());

    std::cout << "result: ";
    std::for_each(v.begin(), v.end(), [](int &v) {
        std::cout << v << ", ";
    });

    std::cout << std::endl;
    
    return 0;
}
