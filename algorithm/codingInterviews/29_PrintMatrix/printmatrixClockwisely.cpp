/*************************************************************************
 > File Name: printmatrixClockwisely.cpp
 > Created Time: 2018年04月12日 星期四 16时33分46秒
 ************************************************************************/

#include <iostream>

void printMatrixClockwisely(int* matrix, int rows, int cols)
{
    int row = 0, col = 0;
    int endRow = rows-1, endCol = cols-1;

    std::cout << "dump: ";
    for (;;) {
        for (int i = col; i <= endCol; ++i) {
            std::cout << matrix[row*cols+i] << " ";
        }
        if (++row > endRow)
            break;

        for (int i = row; i <= endRow; ++i) {
            std::cout << matrix[i*cols+endCol] << " ";
        }
        if (col > --endCol)
            break;

        for (int i = endCol; i >= col; --i) {
            std::cout << matrix[endRow*cols+i] << " ";
        }
        if (row > --endRow)
            break;

        for (int i = endRow; i >= row; --i) {
            std::cout << matrix[i*cols+col] << " ";
        }
        if (++col > endCol)
            break;
    }
    std::cout << std::endl;
}


int main(int argc, char** argv)
{
    //int matrix[][5] = {
        //{1, 2, 3, 4, 5},
        //{6, 7, 8, 9, 10},
        //{11, 12, 13, 14, 15},
        //{16, 17, 18, 19, 20},
    //};

    //int matrix[] = {1, 2, 3, 4};

    //int matrix[][4] = {5, 6, 7, 8};

    //int matrix[][4] = {1, 2, 3, 4, 5, 6, 7, 8};

    int matrix[][2] = {1, 2, 3, 4, 5, 6, 7, 8};

    printMatrixClockwisely((int*)matrix, 4, 2);

    return 0;
}

