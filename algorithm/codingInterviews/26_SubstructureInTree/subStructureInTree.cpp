/*************************************************************************
 > File Name: subStructureInTree.cpp
 > Created Time: 2018年04月11日 星期三 22时32分01秒
 ************************************************************************/

#include <iostream>

bool equal(double num1, double num2)
{
    if (num1-num2>-1e-7 && num1-num2<1e-7)
        return true;
    else
        return false;
}

struct BinaryTreeNode {
    BinaryTreeNode(double v)
    : mValue(v)
    , left(nullptr)
    , right(nullptr) {}
    double mValue;
    BinaryTreeNode* left;
    BinaryTreeNode* right;
};

bool doHasSubTree(BinaryTreeNode* root1, BinaryTreeNode* root2)
{
    bool result = false;

    if (root2 == nullptr) {
        return true;
    }

    if (root1 == nullptr) {
        return false;
    }

    if (equal(root1->mValue, root2->mValue)) {
        result = true;
    }

    if (result) {
        result = doHasSubTree(root1->left, root2->left) &&
            doHasSubTree(root1->right, root2->right);
    }

    return result;
}

bool hasSubTree(BinaryTreeNode* root1, BinaryTreeNode* root2)
{
    bool result = false;

    if (root1 == nullptr || root2 == nullptr)
        return false;

    if (equal(root1->mValue, root2->mValue)) {
        result = doHasSubTree(root1, root2);
    }

    if (!result) {
        result = hasSubTree(root1->left, root2);
    }

    if (!result) {
        result = hasSubTree(root1->right, root2);
    }

    return result;
}


int main(int argc, char** argv)
{
    BinaryTreeNode* node1 = new BinaryTreeNode(1.0);
    BinaryTreeNode* node2 = new BinaryTreeNode(2.0);
    BinaryTreeNode* node3 = new BinaryTreeNode(3.0);
    node1->left = node2;
    node1->right = node3;

    BinaryTreeNode* node4 = new BinaryTreeNode(4.0);


    std::cout << std::boolalpha << hasSubTree(node1, node4) << std::endl;

    return 0;
}

