/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

struct BinaryTreeNode {
    BinaryTreeNode(int v)
    : mValue(v)
    , left(nullptr)
    , right(nullptr) {}
    int mValue;
    BinaryTreeNode* left;
    BinaryTreeNode* right;
};

void findPath(BinaryTreeNode* root, int expectedSum, std::vector<BinaryTreeNode*> &s, int &currentSum)
{
    currentSum += root->mValue;
    s.push_back(root);

    if (root->left == nullptr && root->right == nullptr) {
        if (currentSum == expectedSum) {
            std::cout << "dump: ";
            std::for_each(s.begin(), s.end(), [](BinaryTreeNode* node) {
                    std::cout << node->mValue << " ";
                    });
            std::cout << std::endl;
        }
    } else {
        if (root->left) {
            findPath(root->left, expectedSum, s, currentSum);
        }

        if (root->right) {
            findPath(root->right, expectedSum, s, currentSum);
        }
    }

    currentSum -= root->mValue;
    s.pop_back();
}

void findPath(BinaryTreeNode* root, int expectedSum)
{
    if (root == nullptr)
        return;

    std::vector<BinaryTreeNode*> s;
    int currentSum = 0;
    findPath(root, expectedSum, s, currentSum);
    assert(currentSum==0);
    assert(s.size()==0);
}


int main(int argc, char** argv)
{
    BinaryTreeNode* n1 = new BinaryTreeNode(10);
    BinaryTreeNode* n2 = new BinaryTreeNode(5);
    BinaryTreeNode* n3 = new BinaryTreeNode(12);
    BinaryTreeNode* n4 = new BinaryTreeNode(4);
    BinaryTreeNode* n5 = new BinaryTreeNode(7);

    n1->left = n2;
    n1->right = n3;
    n2->left = n4;
    n2->right = n5;

    findPath(n1, 22);

    return 0;
}
