/*************************************************************************
 > File Name: printTreeFromTopToBotom.cpp
 > Created Time: 2018年04月12日 星期四 23时00分20秒
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

void levelPrint(BinaryTreeNode* root)
{
    std::queue<BinaryTreeNode*> q;
    BinaryTreeNode* node;


    std::cout << "level print: ";

    q.push(root);
    while (q.size()) {
        node = q.front();
        q.pop();
        std::cout << node->mValue << " ";
        if (node->left) {
            q.push(node->left);
        }
        if (node->right) {
            q.push(node->right);
        }
    }
    std::cout << std::endl;
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

    levelPrint(n1);

    return 0;
}

