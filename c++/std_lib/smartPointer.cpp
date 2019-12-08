#include <functional>
#include <memory>
#include <vector>
#include <iostream>

int test_smartPointer()
{
    std::unique_ptr<int> pi;
    std::vector<std::unique_ptr<int>> v;

    v.push_back(std::move(pi));


    for (auto &p : v) {
        std::cout << p.get() << std::endl;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    test_smartPointer();
    
    return 0;
}
