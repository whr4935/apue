/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>

int getFirstK(int* data, int length, int k)
{
    int ret = -1;
    int start = 0;
    int end = length-1;
    int mid = (start+end)>>1;

    while (start <= end) {
        mid = (start+end)>>1;
        if (data[mid]==k) {
            if (mid==start || data[mid-1]!=k) {
                ret = mid;
                break;
            }

            end = mid - 1;
            continue;
        }

        if (data[mid] > k) {
            end = mid-1;
        } else {
            start = mid+1;
        }
    }

    return ret;
}

int getLastK(int* data, int length, int k)
{
    int ret = -1;
    int start = 0;
    int end = length-1;
    int mid = (start+end)>>1;

    while (start <= end) {
        mid = (start+end)>>1;
        if (data[mid]==k) {
            if (mid==end || data[mid+1]!=k) {
                ret = mid;
                break;
            }

            start = mid + 1;
            continue;
        }

        if (data[mid] > k) {
            end = mid-1;
        } else {
            start = mid+1;
        }
    }

    return ret;
}

int getNumberOfK(int* data, int length, int k)
{
    int ret = -1;

    int l = getFirstK(data, length, k);
    int r = getLastK(data, length, k);

    if (l>-1 && r >-1) {
        ret = r-l+1;
    }

    return ret;
}

int main(int argc, char *argv[])
{
    int data[] = {1, 2, 3, 3, 3, 3, 4, 5};
    std::cout << getNumberOfK(data, sizeof(data)/sizeof(*data), 3) << std::endl;

    return 0;
}
