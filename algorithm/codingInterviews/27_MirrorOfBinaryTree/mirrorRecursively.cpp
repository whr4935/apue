/*************************************************************************
 > File Name: mirrorRecursively.cpp
 > Created Time: 2018年04月12日 星期四 13时45分48秒
 ************************************************************************/

#include <iostream>
#include <algorithm>

struct BinaryTreeNode {
    BinaryTreeNode(int v)
    : mValue(v)
    , left(nullptr)
    , right(nullptr) {}
    int mValue;
    BinaryTreeNode* left;
    BinaryTreeNode* right;
};

void mirrorRecursively(BinaryTreeNode* root)
{
    if (root == nullptr)
        return;

    std::swap(root->left, root->right);

    if (root->left)
        mirrorRecursively(root->left);

    if (root->right)
        mirrorRecursively(root->right);
}

void dump(BinaryTreeNode* root)
{
    if (root == nullptr)
        return;

    std::cout << root->mValue << " ";
    if (root->left)
        dump(root->left);

    if (root->right)
        dump(root->right);
}

int main(int argc, char** argv)
{
    BinaryTreeNode* n1 = new BinaryTreeNode(1);
    BinaryTreeNode* n2 = new BinaryTreeNode(2);
    BinaryTreeNode* n3 = new BinaryTreeNode(3);
    BinaryTreeNode* n4 = new BinaryTreeNode(4);
    BinaryTreeNode* n5 = new BinaryTreeNode(5);

    n1->left = n2;
    n1->right = n3;
    n2->left = n4;
    n3->right = n5;

    std::cout << "dump: ";
    dump(n1);
    std::cout << std::endl;

    mirrorRecursively(n1);

    std::cout << "dump: ";
    dump(n1);
    std::cout << std::endl;

    return 0;
}

