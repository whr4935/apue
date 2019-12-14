#include <iostream>
#include <exception>
#include <algorithm>

struct BinaryTreeNode {
    int m_nValue;
    BinaryTreeNode* m_pLeft;
    BinaryTreeNode* m_pRight;

    BinaryTreeNode(int value = -1) 
    : m_nValue(value)
    , m_pLeft(nullptr)
    , m_pRight(nullptr)
    {
    }
};

BinaryTreeNode* constructCore(int* startPreOrder, int* endPreOrder, int* startInOrder, int* endInOrder)
{
    BinaryTreeNode* root = new BinaryTreeNode(startPreOrder[0]);

    int* p = startInOrder;
    while (p <= endInOrder && root->m_nValue != *p)
        ++p;

    if (p > endInOrder) {
        throw std::exception();
    }
    int leftNum = p - startInOrder;

    if (leftNum > 0) {
        root->m_pLeft = constructCore(startPreOrder + 1, startPreOrder + leftNum, startInOrder, p - 1);
    }

    if (endInOrder > p) {
        root->m_pRight = constructCore(startPreOrder + 1 + leftNum, endPreOrder, p + 1, endInOrder);
    }

    return root;
}

BinaryTreeNode* construct(int* preorder, int* inorder, int length)
{
    BinaryTreeNode* root = nullptr;

    root = constructCore(preorder, preorder + length - 1, inorder, inorder + length - 1);

    return root;
}

void preForeach(BinaryTreeNode* root)
{
    if (root == nullptr)
        return;

    std::cout << root->m_nValue << " ";
    if (root->m_pLeft) {
        preForeach(root->m_pLeft);
    }

    if (root->m_pRight) {
        preForeach(root->m_pRight);
    }
}

void inForeach(BinaryTreeNode* root)
{
    if (root == nullptr)
        return;

    if (root->m_pLeft) {
        inForeach(root->m_pLeft);
    }

    std::cout << root->m_nValue << " ";

    if (root->m_pRight) {
        inForeach(root->m_pRight);
    }
}

int main(int argc, char** argv)
{
    int preorder[] = {1, 2, 4, 7, 3, 5, 6, 8};
    int inorder[] = {4, 7, 2, 1, 5, 3, 8, 6};

    std::cout << "preorder:";
    std::for_each(std::begin(preorder), std::end(preorder), [](int v) {std::cout << v << " ";});
    std::cout << std::endl;

    std::cout << "inorder:";
    std::for_each(std::begin(inorder), std::end(inorder), [](int v) {std::cout << v << " ";});
    std::cout << std::endl;

    BinaryTreeNode* root = construct(preorder, inorder, sizeof(preorder) / sizeof(*preorder));
    std::cout << "\n>> after construct:\n" << std::endl;

    std::cout << "preorder:";
    preForeach(root);
    std::cout << std::endl;

    std::cout << "inorder:";
    inForeach(root);
    std::cout << std::endl;

    return 0;
}
