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


int main(int argc, char *argv[])
{
    A a; 
    //Obj &o = (Obj&)a;
    Obj o = (Obj)a;

    if (&o == &a.o) {
        printf("equal\n");
    }

    printf("%p, %p\n", &o, &a.o);


    printf("exit\n");
    return 0;
}
