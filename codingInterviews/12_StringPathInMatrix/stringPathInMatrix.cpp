/*************************************************************************
 > File Name: stringPathInMatrix.cpp
 > Created Time: 2018年04月08日 星期日 22时50分50秒
 ************************************************************************/

#include <iostream>
#include <string.h>

bool hasPathCore(char* matrix, int rows, int cols, int row, int col, char* str, int& length, bool* visited)
{
    bool hasPath = false;

    if (str[length] == '\0')
        return true;

    if (row < 0 || row >= rows || col < 0 || col >= cols || visited[row*cols+col])
        return false;

    if (matrix[row*cols+col] == str[length]) {
        ++length;
        visited[row*cols+col] = true;

        hasPath = hasPathCore(matrix, rows, cols, row-1, col, str, length, visited) ||
            hasPathCore(matrix, rows, cols, row+1, col, str, length, visited) ||
            hasPathCore(matrix, rows, cols, row, col-1, str, length, visited) ||
            hasPathCore(matrix, rows, cols, row, col+1, str, length, visited);

        if (!hasPath) {
            --length;
            visited[row*cols+col] = false;
        }
    }

    return hasPath;
}

bool hasPath(char* matrix, int rows, int cols, char* str)
{
    bool *visited = new bool[rows*cols];
    memset(visited, 0, rows*cols);

    int length = 0;
    bool ret = false;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            ret = hasPathCore(matrix, rows, cols, i, j, str, length, visited);   
            if (ret)
                break;
        }
    }

    delete [] visited;

    return ret;
}

int main(int argc, char** argv)
{
    char matrix[][4] = {
        {'a', 'b', 't', 'g'},
        {'c', 'f', 'c', 's'},
        {'j', 'd', 'e', 'h'}
    };

    char str[] = "bfcf";
    if (hasPath((char*)matrix, 3, 4, str)) {
        std::cout << str << ": has path!" << std::endl;
    } else {
        std::cout << str << ": don't have path!" << std::endl;
    }
    
    return 0;
}

