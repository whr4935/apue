#include <ADT/tree.h>
#include <utils/utils_main/utils_main.h>

using namespace alg;

Tree* init_binary_search_tree()
{
    Tree* head = new Tree(INT32_MIN);
    head->l = head->r = nullptr;

    return head;
}

void destroy_binary_search_tree(Tree* head)
{
    if (head == nullptr) return;

    if (head->l) {
        destroy_binary_search_tree(head->l);
        head->l = nullptr;
    }

    if (head->r) {
        destroy_binary_search_tree(head->r);
        head->r = nullptr;
    }

    delete head;
}

void insert_binary_search_tree(Tree* head, int v, void* info)
{
    Tree* node = new Tree(v, info);

    Tree* p = head;
    Tree* root = head->r;
    while (root) {
        p = root;
        root = v < root->key ? root->l : root->r;
    }

    if (v < p->key) {
        p->l = node;
    } else {
        p->r = node;
    }
}

void* search_binary_search_tree(Tree* head, int v)
{
    head = head->r;

    while (head) {
        if (v == head->key) {
            break;
        } else if (v < head->key) {
            head = head->l;
        } else {
            head = head->r;
        }
    }

    return head != nullptr ? head->value : nullptr;
}

void delete_binary_search_tree(Tree* head, int v)
{
    Tree* p = head;
    Tree* x = head->r;

    while (x) {
        if (v == x->key) {
            break;
        } 
        
        p = x;
        if (v < x->key) {
            x = x->l;
        } else {
            x = x->r;
        }
    }

    if (x == nullptr) {
        return;
    }

    Tree* t = x;
    if (x->r == nullptr) {
        x = x->l;
    } else if (x->r->l == nullptr) {
        x->r->l = x->l;
        x = x->r;
    } else {
        Tree* c = x->r;
        while (c->l->l) {
            c = c->l;
        }
        x = c->l;
        c->l = x->r;
        x->l = t->l;
        x->r = t->r;
    }

    if (v < p->key) {
        p->l = x;
    } else {
        p->r = x;
    }

    delete t;
}

int test_binary_search_tree()
{
    //printf("%s\n", __FUNCTION__);
    Tree* head = init_binary_search_tree();

    char datas[] = {"ASEBRCHI"};

    for (int i = 0; i < sizeof(datas)-1; ++i) {
        int v = datas[i];
        insert_binary_search_tree(head, v, (void*)v);
    }

    print_tree(head->r);


    printf("search R: %s\n", search_binary_search_tree(head, 'R') ? "success!" : "failure!");
    printf("search K: %s\n", search_binary_search_tree(head, 'K') ? "success!" : "failure!");

    char d = 'E';
    printf("delete %c:\n", d);
    delete_binary_search_tree(head, d);
    print_tree(head->r);

    destroy_binary_search_tree(head->r);
    return 0;
}

AUTO_RUN(test_binary_search_tree)
