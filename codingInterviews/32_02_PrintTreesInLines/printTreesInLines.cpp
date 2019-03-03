/*************************************************************************
 > File Name: printTreesInLines.cpp
 > Created Time: 2018年04月13日 星期五 08时08分53秒
 ************************************************************************/

#include <iostream>
#include <queue>

struct BinaryTreeNode {
    BinaryTreeNode(int v)
    : mValue(v)
    , left(nullptr)
    , right(nullptr) {}
    int mValue;
    BinaryTreeNode* left;
    BinaryTreeNode* right;
};

void printTreesInLines(BinaryTreeNode* root)
{
    std::queue<BinaryTreeNode*> q;
    BinaryTreeNode* node;
    int toBePrintNum = 0;
    int nextLevelNum = 0;

    std::cout << "dump:" << std::endl;

    q.push(root);
    toBePrintNum = 1;
    while (q.size()) {
        node = q.front();
        q.pop();

        if (node->left) {
            q.push(node->left);
            ++nextLevelNum;
        }

        if (node->right) {
            q.push(node->right);
            ++nextLevelNum;
        }


        std::cout << node->mValue << " ";
        if (--toBePrintNum == 0) {
            std::cout << std::endl;
            toBePrintNum = nextLevelNum;
            nextLevelNum = 0;
        }
    }
}

int main(int argc, char** argv)
{
    BinaryTreeNode* n1 = new BinaryTreeNode(8);
    BinaryTreeNode* n2 = new BinaryTreeNode(6);
    BinaryTreeNode* n3 = new BinaryTreeNode(10);
    BinaryTreeNode* n4 = new BinaryTreeNode(5);
    BinaryTreeNode* n5 = new BinaryTreeNode(7);
    BinaryTreeNode* n6 = new BinaryTreeNode(9);
    BinaryTreeNode* n7 = new BinaryTreeNode(11);

    n1->left = n2;
    n1->right = n3;
    n2->left = n4;
    n2->right = n5;
    n3->left = n6;
    n3->right = n7;

    printTreesInLines(n1);

    return 0;
}

