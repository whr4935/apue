#include <ADT/tree.h>
#include <utils/utils_main/utils_main.h>

using namespace alg;

static inline int bits(int v, int p, int w)
{
    return (v >> p) & ~(~0 << w);
}

static inline int c2d(char c)
{
    return c - 'A';
}

Tree* init_patricia_tree()
{
    Tree* head = new Tree(INT32_MIN);
    head->l = head->r = nullptr;

    return head;
}

void destroy_patricia_tree(Tree* head)
{
    if (head == nullptr) return;

    if (head->l) {
        destroy_patricia_tree(head->l);
        head->l = nullptr;
    }

    if (head->r) {
        destroy_patricia_tree(head->r);
        head->r = nullptr;
    }

    delete head;
}

void insert_patricia_tree(Tree* head, int v, void* info)
{

}

void* search_patricia_tree(Tree* head, int v)
{

    return nullptr;
}

void delete_patricia_tree(Tree* head, int v)
{

}

int test_patricia_tree()
{
    Tree* head = init_patricia_tree();

    char datas[] = {"ASEBRCHI"};

    for (int i = 0; i < sizeof(datas)-1; ++i) {
        int v = datas[i];
        insert_patricia_tree(head, v, (void*)v);
    }

    print_tree(head->r);


    printf("search R: %s\n", search_patricia_tree(head, 'R') ? "success!" : "failure!");
    printf("search K: %s\n", search_patricia_tree(head, 'K') ? "success!" : "failure!");

    char d = 'E';
    printf("delete %c:\n", d);
    delete_patricia_tree(head, d);
    print_tree(head->r);

    destroy_patricia_tree(head->r);

    return 0;
}


AUTO_RUN(test_patricia_tree)


