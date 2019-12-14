/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>

/**
* @brief
* @param
* @param
* @return
*/
bool verifySequenceOfBST(int sequence[], int length)
{
    int root;
    int delimit = -1;

    if (sequence == nullptr || length == 0)
    if (sequence == nullptr || length == 0)
    if (sequence == nullptr || length == 0)
    if (sequence == nullptr || length == 0)
        return false;

    if (length <= 2)
        return true;

    root = sequence[length-1];
    for (int i = 0; i <= length-1; ++i) {
        if (sequence[i] >= root) {
            delimit = i;
            break;
        }
    }

    if (delimit < 0 || delimit == length-1) {
        return verifySequenceOfBST(sequence, length-1);
    } else {
        for (int i = delimit + 1; i < length - 1; ++i) {
            if (sequence[i] < root)
                return false;
        }
        return verifySequenceOfBST(sequence, delimit) && verifySequenceOfBST(sequence+delimit, length-1-delimit);
    }
}

int main(int argc, char **argv)
{
    //int sequence[] = {5, 7, 6, 9, 11, 10, 8};
    int sequence[] = {7, 4, 6, 5};

    std::cout << std::boolalpha << verifySequenceOfBST(sequence, sizeof(sequence)/sizeof(*sequence)) << std::endl;

    return 0;
}
