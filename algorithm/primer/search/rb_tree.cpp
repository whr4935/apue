#include <ADT/tree.h>
#include <utils/utils_main/utils_main.h>

using namespace alg;

Tree* init_rb_tree()
{
    Tree* head = new Tree(INT32_MIN);
    head->l = head->r = nullptr;

    return head;
}

void destroy_rb_tree(Tree* head)
{
    if (head == nullptr) return;

    if (head->l) {
        destroy_rb_tree(head->l);
        head->l = nullptr;
    }

    if (head->r) {
        destroy_rb_tree(head->r);
        head->r = nullptr;
    }

    delete head;
}

static struct Tree* rotate(int v, Tree* y)
{
    Tree *c, *gc;
    c = v < y->key ? y->l : y->r;

    if (v < c->key) {
        gc = c->l;
        c->l = gc->r;
        gc->r = c;
    } else {
        gc = c->r;
        c->r = gc->l;
        gc->l = c;
    }

    if (v < y->key) {
        y->l = gc;
    } else {
        y->r = gc;
    }

    return gc;
}

static void split(int v, Tree* x, Tree* p, Tree* g, Tree* gg, Tree* head)
{
    x->red = 1;
    if (x->l) x->l->red = 0;
    if (x->r) x->r->red = 0;

    if (p->red) {
        g->red = 1;
        if (v < g->key != v < p->key) {
            p = rotate(v, g);
        }
        x = rotate(v, gg);
        x->red = 0;
    }
    head->r->red = 0;
}

void insert_rb_tree(Tree* head, int v, void* info)
{
    Tree* x = head->r;
    Tree* p = head;
    Tree* g = head;
    Tree* gg = head;

    while (x != nullptr) {
        gg = g;
        g = p;
        p = x;

        x = (v < x->key) ? x->l : x->r;
        if (x && x->l && x->l->red && x->r && x->r->red) {
            split(v, x, p, g, gg, head);
        }
    }

    x = new Tree(v, info);
    if (v < p->key) {
        p->l = x;
    } else {
        p->r = x;
    }
    split(v, x, p, g, gg, head);
}

void* search_rb_tree(Tree* head, int v)
{
    Tree* x = head->r;

    while (x) {
        if (v == x->key) {
            break;
        }

        x = v < x->key ? x->l : x->r;
    }

    return x != nullptr ? x->value : nullptr;
}

void delete_rb_tree(Tree* head, int v)
{
    Tree* p = head;
    Tree* x = head->r;

    while (x) {
        head = x;
        if (v == x->key) {
            break;
        }

        x = v < x->key ? x->l : x->r;
    }

    if (x == nullptr) {
        return;
    }

    Tree* t = x;
    if (t->r == nullptr) {
        x = t->l;
    } else if (t->r->l == nullptr) {
        x = t->r;
        x->l = t->l;
    } else {
        Tree* c = t->r;
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

int test_rb_tree()
{
    //printf("%s\n", __FUNCTION__);
    Tree* head = init_rb_tree();

    char datas[] = {"ASEBRCHI"};

    for (int i = 0; i < sizeof(datas)-1; ++i) {
        int v = datas[i];
        insert_rb_tree(head, v, (void*)v);
    }

    print_tree(head->r);


    printf("search R: %s\n", search_rb_tree(head, 'R') ? "success!" : "failure!");
    printf("search K: %s\n", search_rb_tree(head, 'K') ? "success!" : "failure!");

    char d = 'E';
    printf("delete %c:\n", d);
    delete_rb_tree(head, d);
    print_tree(head->r);

    destroy_rb_tree(head->r);
    return 0;
}

AUTO_RUN(test_rb_tree)
