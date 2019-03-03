/*************************************************************************
 > File Name: printTreeInZigZag.cpp
 > Created Time: 2018年04月13日 星期五 08时18分26秒
 ************************************************************************/

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

void printTreeInZigZag(BinaryTreeNode* root)
{
    std::stack<BinaryTreeNode*> s1;
    std::stack<BinaryTreeNode*> s2;
    BinaryTreeNode* node;
    int toBePrintNum = 0;
    int nextLevelNum = 0;
    int level = 1;

    if (level&1)
        s1.push(root);
    else
        s2.push(root);
    toBePrintNum = 1;
    while (s1.size() || s2.size()) {
        if ((level&1) == 0) {
            node = s2.top();
            s2.pop();

            if (node->left) {
                s1.push(node->left);
                ++nextLevelNum;
            }

            if (node->right) {
                s1.push(node->right);
                ++nextLevelNum;
            }

            std::cout << node->mValue << " ";
            if (--toBePrintNum == 0) {
                std::cout << std::endl;
                toBePrintNum = nextLevelNum;
                nextLevelNum = 0;
                ++level;
            }
        } else {
            node = s1.top();
            s1.pop();

            if (node->right) {
                s2.push(node->right);
                ++nextLevelNum;
            }

            if (node->left) {
                s2.push(node->left);
                ++nextLevelNum;
            }

            std::cout << node->mValue << " ";
            if (--toBePrintNum == 0) {
                std::cout << std::endl;
                toBePrintNum = nextLevelNum;
                nextLevelNum = 0;
                ++level;
            }
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

    printTreeInZigZag(n1);

    return 0;
}

