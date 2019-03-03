#ifndef TREE_H
#define TREE_H

struct tree_node {
    int key;
    int pos;
    int depth;
    struct tree_node *l, *r;
};


void print_tree();

int test_tree();

#endif // TREE_H
