#include <iostream>

struct A
{
    A() {}

    //A& operator=(const A& rhs) {
        //return *this;
    //}

    //A(const A&) {

    //}

    A& operator=(A&& rhs) {
        return *this;
    }

    //A(A&& other) {

    //}

    int mData = 10;
};


int main(int argc, char *argv[])
{
    A a;
    A b;
    
    
    A a2 = a;
    b = a;

    b = std::move(a);

    return 0;
}
