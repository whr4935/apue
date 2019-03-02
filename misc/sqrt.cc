#include <stdio.h>

template<typename T>
T abs(T d)
{
    return d < 0 ? -d : d;
}

double mysqrt(double d)
{
    double low = 0;
    double hi = d;
    double mid = d;
    double last = d;
    do {
        if (mid*mid > d) {
            hi = mid;
        } else {
            low = mid;
        } 
        last = mid;
        mid = (low+hi)/2;
    } while (abs(mid-last) > 0.001);

    return mid;
}

int main(int argc, char *argv[])
{
    printf("sqrt test: %f\n", mysqrt(3));

    return 0;
}
