/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <exception>

template<typename T>
class DynamicAray {
public:
    void insert(T num) {
        if (((min.size() + max.size())&1) == 0) {
            if (max.size() > 0 && num < max[0]) {
                max.push_back(num);
                std::push_heap(max.begin(), max.end(), std::less<T>());
                num = max[0];
                std::pop_heap(max.begin(), max.end(), std::less<T>());
                max.pop_back();
            }
            min.push_back(num);
            std::push_heap(min.begin(), min.end(), std::greater<T>()); 
        } else {
            if (min.size() > 0 && num > min[0]) {
                min.push_back(num);
                std::push_heap(min.begin(), min.end(), std::greater<T>());
                num = min[0];
                std::pop_heap(min.begin(), min.end(), std::greater<T>());
                min.pop_back();
            }
            max.push_back(num);
            std::push_heap(max.begin(), max.end(), std::less<T>());
        }
    }

    T getMedian() {
        if (min.size() + max.size() == 0) {
            throw std::logic_error("error!");
        }

        T median;
        if (((min.size() + max.size())&1) == 0) {
            median = (max[0] + min[0])/2;
        } else {
            median = min[0];
        }
        return median;
    }

private:
    std::vector<T> min;
    std::vector<T> max;
};

int main(int argc, char *argv[])
{
    int data[] = {1, 4, 9, 6, 2};

    DynamicAray<int> d;
    for (size_t i = 0; i < sizeof(data)/sizeof(*data); ++i) {
        d.insert(data[i]);
    }

    std::cout << d.getMedian() << std::endl;

    return 0;
}
