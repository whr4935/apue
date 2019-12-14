/*
 * Copyright (C) Harry Wang
 *
 * Create Time: 2018.04
 */

#include <iostream>
#include <sstream>

struct BinaryTreeNode {
    BinaryTreeNode(int v)
    : mValue(v)
    , left(nullptr)
    , right(nullptr) {}
    int mValue;
    BinaryTreeNode* left;
    BinaryTreeNode* right;
};

template <typename out_type, typename in_type=std::string>
out_type convert(const in_type& v)
{
    std::stringstream ss;
    out_type result;
    ss << v;
    ss >> result;

    return result;
}

void serialize(BinaryTreeNode* pRoot, std::ostream &stream)
{
    if (pRoot == nullptr) {
        stream << ' ' + convert<std::string>('$');
        return;
    }

    stream << ' ' + convert<std::string>(pRoot->mValue);
    serialize(pRoot->left, stream);
    serialize(pRoot->right, stream);
}


BinaryTreeNode* unserialize(std::stringstream &stream)
{
    BinaryTreeNode* root = nullptr;
    //char s[4];
    std::string s;

    //stream.getline(s, 4, ' ');
    //std::cout << std::string(s) << std::endl;
    stream >> s;
    if (stream) {
        if (s[0] == '$') {
            root = nullptr;
        } else {
            root = new BinaryTreeNode(convert<int>(std::string(s)));
        }
    }
    
    if (root) {
        root->left = unserialize(stream);
        root->right = unserialize(stream);
    }
    return root;
}

void dump(BinaryTreeNode* root)
{
    if (root == nullptr)
        return;

    std::cout << root->mValue << " ";

    if (root->left) {
        dump(root->left);
    }

    if (root->right) {
        dump(root->right);
    }
}

int main(int argc, char *argv[])
{
    BinaryTreeNode* root = new BinaryTreeNode(1);
    BinaryTreeNode* n2 = new BinaryTreeNode(2);
    BinaryTreeNode* n3 = new BinaryTreeNode(3);
    BinaryTreeNode* n4 = new BinaryTreeNode(4);
    BinaryTreeNode* n5 = new BinaryTreeNode(5);
    BinaryTreeNode* n6 = new BinaryTreeNode(6);

    root->left = n2;
    root->right = n3;
    n2->left = n4;
    n3->left = n5;
    n3->right = n6;

    std::stringstream os;
    serialize(root, os);

    os.seekg(0);
    std::cout << "dump:";
    //while (os) {
        //int s;
        //os >> s; 
        //std::cout << s << " ";
    //}
    std::cout << os.str();
    std::cout << std::endl;

    std::cout << "unserialize:";
    os.seekp(0);
    root = unserialize(os);
    dump(root);

    return 0;
}
