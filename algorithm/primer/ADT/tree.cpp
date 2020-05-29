#include "tree.h"
#include <string.h>
#include <stdio.h>
#include <map>
#include <queue>

namespace alg {

struct NodeMeta {
    int pos;
    int depth;

    NodeMeta(int _pos, int _depth)
    : pos(_pos)
    , depth(_depth)
    {}
};


void print_tree(Tree* root)
{
    const int mid_pos = 50;
    const int step = 2;

    std::map<Tree*, NodeMeta> metas;
    std::queue<Tree*> tree_nodes;
    char buf[2][100];
    Tree* node = nullptr;
    int cur_depth = 0;
    int cur_pos = mid_pos;

    memset(buf, ' ', 200);

    metas.emplace(root, NodeMeta(cur_pos, cur_depth));
    tree_nodes.push(root);

    NodeMeta* m;
    while (!tree_nodes.empty()) {
        node = tree_nodes.front();
        tree_nodes.pop();
        auto ret = metas.find(node);
        if (ret != metas.end()) {
            m = &ret->second;
        } else {
            printf("find node:%p failed!\n", node);
        }

        if (m->depth != cur_depth) {
            buf[0][99] = 0; buf[1][99] = 0;
            puts(buf[0]);
            puts(buf[1]);
            memset(buf, ' ', 200);
        }

        cur_depth = m->depth;
        cur_pos = m->pos;

        if (buf[0][cur_pos] == ' ') {
            buf[0][cur_pos] = node->key;
        } else {
            buf[0][cur_pos+1] = node->key;
        }
        
        if (node->l) {
            buf[1][cur_pos-step] = '/';
            metas.emplace(node->l, NodeMeta(cur_pos - 2*step , cur_depth+1));
            tree_nodes.push(node->l);
        }

        if (node->r) {
            buf[1][cur_pos+step] = '\\';
            metas.emplace(node->r, NodeMeta(cur_pos + 2*step, cur_depth+1));
            tree_nodes.push(node->r);
        }
    }

    if (cur_depth > 0) {
        buf[0][99] = 0; buf[1][99] = 0;
        puts(buf[0]);
        puts(buf[1]);
        memset(buf, 0, 200);
    }
}


}
