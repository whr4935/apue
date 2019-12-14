/*************************************************************************
 > File Name: robotMove.cpp
 > Created Time: 2018年04月09日 星期一 14时24分22秒
 ************************************************************************/

#include <iostream>
#include <string.h>

int digitAdd(int a)
{
    int sum = 0;
    while (a) {
        sum += a%10;
        a /= 10;
    }

    return sum;
}

int moveCountCore(int threshold, int rows, int cols, int row, int col, bool* visited)
{
    int moveCount = 0;

    if (row < 0 || col < 0 || row >= rows || col >= cols || visited[row*cols+col])
        return moveCount;

    if (digitAdd(row)+digitAdd(col) > threshold) {
        return moveCount;
    }

    visited[row*cols+col] = true;
    moveCount = 1 + moveCountCore(threshold, rows, cols, row+1, col, visited) +
        moveCountCore(threshold, rows, cols, row-1, col, visited) +
        moveCountCore(threshold, rows, cols, row, col+1, visited) +
        moveCountCore(threshold, rows, cols, row, col-1, visited);

    return moveCount;
}

int moveCount(int threshold, int rows, int cols)
{
    bool *visited = new bool[rows*cols];
    memset(visited, 0, rows*cols);

    int moveCount = moveCountCore(threshold, rows, cols, 0, 0, visited);

    delete [] visited;

    return moveCount;
}

int main(int argc, char** argv)
{
    int threshold = 1;
    int rows = 40;
    int cols = 40;
    
    std::cout << moveCount(threshold, rows, cols) << std::endl;
    
    return 0;
}

