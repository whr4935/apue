/*************************************************************************
 > File Name: 04_FindInPartiallySortedMatrix/findInPartiallySortedMatrix.cpp
 > Created Time: 2018年03月27日 星期二 23时32分06秒
 ************************************************************************/
#include <iostream>

bool find(int matrix[][4], int rows, int columns, int number)
{
    int i, j = columns;
    for (i = 0; i < rows; ++i) {
        for (j = columns-1; j >= 0; --j) {
            if (matrix[i][j] == number) {
                std::cout << i << ", " << j << std::endl;
                return true;
            } else if (matrix[i][j] < number) {
                break;
            }
        }
    }

    return false;
}

int main()
{
    int matrix[][4] = {
    {1, 2, 8, 9},
    {2, 4, 9, 12},
    {4, 7, 10, 13},
    {6, 8, 11, 15},
    };

    if (find(matrix, 4, 4, 7)) {
        std::cout << "find success!" << std::endl;
    } else {
        std::cout << "failure!" << std::endl;
    }

    return 0;
}
