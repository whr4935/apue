#include <unistd.h>

func1()
{
    int i = 0;
    for (i = 0; i < 1000; ++i) {
        ;
        int b = i*2;
        b += 4;

    }
}

int main(int argc, char *argv[])
{
    sleep(2);
    func1();
    return 0;
}
