#include <stdio.h>
struct Obj
{
    Obj() {
        printf("ctor\n");
    }

    ~Obj() {
        printf("~dtor\n");
    }

    int mData = 11;
};


struct A
{
    operator Obj()  {
        return o;
    }

    Obj o;
};

template <typename T>
T& getRef(T* p)
{
    return static_cast<T&>(*p);
}


int main(int argc, char *argv[])
{
    A a; 
    //Obj &o = (Obj&)a;
    Obj o = (Obj)a;

    if (&o == &a.o) {
        printf("equal\n");
    }

    printf("%p, %p\n", &o, &a.o);

    A* pa = &a;

    //auto pa2 = getRef(pa); 
    printf("a addr:%p\n", &a);
    printf("test ref: %p\n", &getRef(pa));

    decltype(*pa) pa3 = a;;
    printf("pa3 addr:%p\n", &pa3);


    printf("exit\n");
    return 0;
}
