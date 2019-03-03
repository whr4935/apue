/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>
#include <stack>

struct BinaryTreeNode {
    BinaryTreeNode(int v)
    : mValue(v)
    , left(nullptr)
    , right(nullptr) {}

    int mValue;
    BinaryTreeNode* left;
    BinaryTreeNode* right;
};

BinaryTreeNode* kthNode(BinaryTreeNode* root, int k)
{
    BinaryTreeNode* n = nullptr;
    std::stack<BinaryTreeNode*> s;

    while (root || s.size()) {
        while (root) {
            s.push(root);
            root = root->left;
        }

        if (s.size()) {
            root = s.top();
            s.pop();
        }

        std::cout << root->mValue << " ";
        root = root->right;
    }
    std::cout << std::endl;


    return n;
}

int main(int argc, char *argv[])
{
    BinaryTreeNode* n5 = new BinaryTreeNode(5);
    BinaryTreeNode* n3 = new BinaryTreeNode(3);
    BinaryTreeNode* n7 = new BinaryTreeNode(7);
    BinaryTreeNode* n2 = new BinaryTreeNode(2);
    BinaryTreeNode* n4 = new BinaryTreeNode(4);
    BinaryTreeNode* n6 = new BinaryTreeNode(6);
    BinaryTreeNode* n8 = new BinaryTreeNode(8);

    n5->left = n3;
    n5->right = n7;
    n3->left = n2;
    n3->right = n4;
    n7->left = n6;
    n7->right = n8;

    BinaryTreeNode* n = kthNode(n5, 3);
    if (n) {
        std::cout << n->mValue << std::endl;
    } else {
        std::cout << "not found!" << std::endl;
    }

    return 0;
}
