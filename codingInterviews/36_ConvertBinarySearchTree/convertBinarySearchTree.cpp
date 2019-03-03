/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>

struct BinaryTreeNode {
    BinaryTreeNode(int v)
    : mValue(v)
    , left(nullptr)
    , right(nullptr) {}
    int mValue;
    BinaryTreeNode* left;
    BinaryTreeNode* right;
};

void convertCore(BinaryTreeNode* node, BinaryTreeNode** l, BinaryTreeNode** r)
{
    BinaryTreeNode* ll, *lr, *rl, *rr;

    ll = node;
    lr = node;
    rl = node;
    rr = node;

    if (node->left) {
        convertCore(node->left, &ll, &lr);
    }

    if (node->right) {
        convertCore(node->right, &rl, &rr);
    }

    node->right = rl;
    rl->left = node;
    node->left = lr;
    lr->right = node;

    *l = ll;
    *r = rr;
}

BinaryTreeNode* convert(BinaryTreeNode* pRoot)
{
    BinaryTreeNode *ll = nullptr, *rr=nullptr;
    if (pRoot) convertCore(pRoot, &ll, &rr);
    if (ll != nullptr) ll->left = nullptr;
    if (rr != nullptr) rr->right = nullptr;
    
    return ll;
}

void dump(BinaryTreeNode* root)
{
    std::cout << "dump: ";
    while (root) {
        std::cout << root->mValue << " ";
        root = root->right;
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    BinaryTreeNode* root = new BinaryTreeNode(10);
    BinaryTreeNode* n2   = new BinaryTreeNode(6);
    BinaryTreeNode* n3   = new BinaryTreeNode(14);
    BinaryTreeNode* n4   = new BinaryTreeNode(4);
    BinaryTreeNode* n5   = new BinaryTreeNode(8);
    BinaryTreeNode* n6   = new BinaryTreeNode(12);
    BinaryTreeNode* n7   = new BinaryTreeNode(16);

    root->left  = n2;
    root->right = n3;
    n2->left    = n4;
    n2->right   = n5;
    n3->left    = n6;
    n3->right   = n7;

    auto p = convert(root);
    dump(p);

    return 0;
}
