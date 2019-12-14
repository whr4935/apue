/*************************************************************************
 > File Name: print1ToMaxOfNDigits.cpp
 > Created Time: 2018年04月09日 星期一 22时14分16秒
 ************************************************************************/

#include <iostream>
#include <cstring>

bool increment(char *number)
{
    int len = strlen(number);
    int over = 0;
    bool exceed = false;
    int sum = 0;
    
    for (int i = len-1; i >= 0; --i) {
        sum = number[i]-'0' + over;
        if (i == len-1) {
            ++sum;
        }

        if (sum >= 10) {
            if (i == 0) {
                exceed = true;
            } else {
                sum -= 10;
                over = 1;
                number[i] = sum + '0';
            }
        } else {
            number[i] = sum + '0';
            break;
        }
    }
    
    return exceed;
}

void printNumber(char* number, int len)
{
    int i = 0;

    while (i < len) {
        if (number[i]!='0')
            break;
        ++i;
    }

    if (i < len)
        printf("%s\n", number+i);
}

int main(int argc, char** argv)
{
    int len = 3;
    char *number = new char[len+1];
    for (int i = 0; i < len; ++i) {
        number[i] = '0';
    }

    while (!increment(number)) {
        printNumber(number, len);
    }

    delete number;

    return 0;
}
