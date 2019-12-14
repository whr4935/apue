/*************************************************************************
 > File Name: nextNodeInBinary.cpp
 > Created Time: 2018年03月31日 星期六 22时53分02秒
 ************************************************************************/

#include <iostream>

//线索二叉树
struct TreeNode {
    int value;
    TreeNode *left;
    TreeNode *right;
    bool leftTag;
    bool rightTag;

    TreeNode(int value) {
        this->value = value;
        this->left = nullptr;
        this->right = nullptr;
        this->leftTag = false;
        this->rightTag = false;
    }
};

//static TreeNode* pre = nullptr;
void inTraverse(TreeNode* root, TreeNode** pre)
{
    if (root) {
        inTraverse(root->left, pre);

        if (root->left == nullptr) {
            root->left = *pre;
            root->leftTag = true;
        }


        if (*pre && (*pre)->right == nullptr) {
            (*pre)->right = root;
            (*pre)->rightTag = true;
        }

        *pre = root;
        inTraverse(root->right, pre);
    }
}

void inPrintTraverse(TreeNode* root)
{
    TreeNode* node = root;

    std::cout << "inPrintTraverse: ";
    while (node) {
        while (!node->leftTag && node->left)
           node = node->left; 
        
        std::cout << node->value << " ";

        while (node->rightTag && node->right) {
            node = node->right;
            std::cout << node->value << " ";
        }

        node = node->right;
    }
    std::cout << std::endl;
}

void inReversePrintTraverse(TreeNode* root)
{
    TreeNode* node = root;

    std::cout << "inReversePrintTraverse: ";
    while (node) {
        while (!node->rightTag && node->right) {
            node = node->right;
        }
        std::cout << node->value << " ";

        while (node->leftTag && node->left) {
            node = node->left;
            std::cout << node->value << " ";
        }
        
        node = node->left;
    }
    std::cout << std::endl;
}

///////////////////////////////////////////
void inForEach(TreeNode* root)
{
    if (root) {
        if (!root->leftTag)
            inForEach(root->left);

        std::cout << root->value << " ";

        if (!root->rightTag)
            inForEach(root->right);
    }
}

void inPrint(TreeNode* root)
{
    std::cout << "inForEach: ";
    inForEach(root);
    std::cout << std::endl;
}

int main(int argc, char** argv)
{
    TreeNode* root = new TreeNode(1);

    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);

    inPrint(root);
    TreeNode* pre = nullptr;
    inTraverse(root, &pre);
    // inPrint(root);
    inPrintTraverse(root);
    inReversePrintTraverse(root);



    return 0;
}

