#include <iostream>
#include <stack>

struct BinaryTree {
    int value;
    BinaryTree* left;
    BinaryTree* right;

    BinaryTree(int v)
    : value(v)
    , left(nullptr)
    , right(nullptr)
    {}
};

void preTraverse(struct BinaryTree* root)
{
    struct BinaryTree* node = nullptr;
    std::stack<BinaryTree*> s;

    std::cout << "preTraverse: ";
    node = root;
    while (s.size() || node) {
        while (node) {
            std::cout << node->value << " ";
            if (node->right)
                s.push(node->right);
            node = node->left;
        }

        if (s.size()) {
            node = s.top(); s.pop();
        }
    }
    std::cout << std::endl;
}

void inTraverse(struct BinaryTree* root)
{
    struct BinaryTree* node = root;
    std::stack<BinaryTree*> s;
    std::cout << "inTraverse: ";
    
    while (s.size() || node) {
        while (node) {
            s.push(node);
            node = node->left; 
        }

        node = s.top(); s.pop();
        std::cout << node->value << " ";

        node = node->right;
    }
    std::cout << std::endl;
}

void postTraverse(struct BinaryTree* root)
{
    struct BinaryTree* node = root;
    std::stack<BinaryTree*> s;
    std::stack<BinaryTree*> s2;
    std::cout << "postTraverse: ";
    
    while (s.size() || node) {
        while (node) {
            s.push(node);
            node =node->left;
        }

        node = s.top(); s.pop();
        if (s2.size() && s2.top() == node) {
            s2.pop();
            std::cout << node->value << " ";
            node = nullptr;
        } else {
            s.push(node);
            s2.push(node);
            node = node->right;
        }
    }

    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    struct BinaryTree* root = new BinaryTree(1);
    struct BinaryTree* node2 = new BinaryTree(2);
    struct BinaryTree* node3 = new BinaryTree(3);
    struct BinaryTree* node4 = new BinaryTree(4);
    struct BinaryTree* node5 = new BinaryTree(5);

    root->left = node2;
    root->right = node3;
    node2->right = node4;
    node4->left = node5;
    
    preTraverse(root);
    inTraverse(root);
    postTraverse(root);

    return 0;
}
