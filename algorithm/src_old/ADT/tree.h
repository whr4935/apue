#ifndef _TREE_H
#define	_TREE_H

#include <algorithm.h>

struct Tree {
    element_type	data;
    BOOL			isVisited;
    struct Tree*	first_child;
    struct Tree*	next_sibling;
};

//////////////////////////////////////////////////////////////////////////
struct Tree* mallocTreeNode();
STATUS freeTreeNode(struct Tree* node);
STATUS initTreeNode(struct Tree* node, element_type data);
STATUS initTree(struct Tree **T);


#endif