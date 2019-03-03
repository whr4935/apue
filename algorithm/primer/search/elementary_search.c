#include <includes.h>
#include "elementary_search.h"
#include "ADT/stack.h"
#include "ADT/queue.h"

int seq_search(const int a[], int N, int v)
{
    int i;

    for (i = 0; i < N; ++i) {
        if (a[i] == v)
            return i;
    }

    return -1;
}

void test_seq_search()
{
    int data[] = {1, 33, 2, 190, 9, 102, 33, 76, 28, 11, 19, 95, 27};
    int index;

    insert_sort(data, sizeof(data)/sizeof(*data));
    print_array(data, ARRAY_SIZE(data));

    printf("seq_search:\n");
    index = seq_search(data, sizeof(data)/sizeof(*data), 19);

    printf("index = %d, value = %d\n", index, index>=0 ? data[index] : -1);

    /////////////
    int size = 1024 *1024 * 10 ;
    int *test_data = malloc(size * sizeof(int));
    int i;

    test_data[0] = test_data[1] = 1;
    for (i = 2; i < size; ++i) {
        test_data[i] = test_data[i-2] + test_data[i-1];
    }

    quick_sort(test_data, size);
    int test_index = select_kth(test_data, size, size-200);
    printf("test_index = %d\n", test_index);

    BenchMark b;
    benchmark_begin(&b);

    index = seq_search(test_data, size, test_data[test_index]);

    benchmark_end(&b, "seq_search");

    printf("index = %d, value = %d\n", index, index>=0 ? test_data[index] : -1);

    free(test_data);
}

//////////////////////////////////////////////
int bin_search(int a[], int N, int v)
{
    int m;
    int l = 0, r = N-1;

    while (r >= l) {
        m = (r + l) / 2;

        if (a[m] == v)
            return m;
        if (a[m] > v)
            r = m - 1;
        else
            l = m + 1;
    }
    return -1;
}

//interpolation search
int bin_search_v2(int a[], int N, int v)
{
    int m;
    int l = 0, r = N-1;
    double pos;

    while (r >= l) {
        pos = (v - a[l]) * 1.0 / (a[r] - a[l]);     //计算精度问题可能导致死循环
        m = l + (r-l) * pos;

        if (a[m] == v)
            return m;
        if (a[m] > v)
            r = m - 1;
        else
            l = m + 1;
    }
    return -1;
}


#define bin_search_func bin_search_v2
void test_bin_search()
{
    int data[] = {1, 33, 2, 190, 9, 102, 33, 76, 28, 11, 19, 95, 27};
    int index;

    insert_sort(data, sizeof(data)/sizeof(*data));
    print_array(data, ARRAY_SIZE(data));

    printf("bin_search:\n");
    index = bin_search_func(data, sizeof(data)/sizeof(*data), 19);

    printf("index = %d, value = %d\n", index, index>=0 ? data[index] : -1);

    /////////////
    int size = 1024 *1024 * 10 ;
    int *test_data = malloc(size * sizeof(int));
    int i;

    test_data[0] = test_data[1] = 1;
    for (i = 2; i < size; ++i) {
        test_data[i] = test_data[i-2] + test_data[i-1];
    }
//    print_array(test_data, 100);

    quick_sort(test_data, size);
    int test_index = select_kth(test_data, size, size-200);
    printf("test_index = %#x\n", test_index);

    BenchMark b;
    benchmark_begin(&b);

    index = bin_search_func(test_data, size, test_data[test_index]);

    benchmark_end(&b, "bin_search");

    printf("index = %d, value = %d\n", index, index>=0 ? test_data[index] : -1);

    free(test_data);
}

//////////////////////////////////////////////
//二叉查找树
struct node {
    int key;
    int info;
    char visited;
    char red;
    struct node *l, *r;
};
static struct node *head, *z;


//////////////////////////////////////////////
void init_bin_tree()
{
    z = (struct node*)calloc(1, sizeof(struct node));
    z->l = z;
    z->r = z;
    z->info = -1;

    head = (struct node*)calloc(1, sizeof(struct node));
    head->r = z;
    head->key = 0;
}

static void destroy_bin_tree_core(struct node* node)
{
    if (node != z) {
        destroy_bin_tree_core(node->l);
        destroy_bin_tree_core(node->r);
        free(node);
    }
}

void destroy_bin_tree()
{
    struct node *x = head->r;

    destroy_bin_tree_core(x);
    free(z);
 //   free(head);
}

void insert_bin_tree(int v, int info)
{
    struct node* x = head->r, *p = head;

    while (x != z) {
        p = x; x = v < x->key ? x->l : x->r;
    }

    x = (struct node*)calloc(1, sizeof(struct node));
    x->key = v;
    x->info = info;
    x->l = z;
    x->r = z;

    if (v < p->key)
        p->l = x;
    else
        p->r = x;
}

//非递归中序遍历
void in_bin_tree(int *in_index)
{
    struct node* x= head->r;
    init_stack();

    printf("sort bin tree:");
    while (x!=z || !stack_empty()) {
        while (x != z) {
            push(x);
            x = x->l;
        }

        x = pop();
        printf("%3d%s ", x->key, x->red?"[r]":"");
        if (in_index)
            *in_index++ = x->info;

        x = x->r;

    }
    printf("\n");
}

//非递归前序遍历
void pre_bin_tree(int *pre_index)
{
    struct node *x = head->r;
    init_stack();

    if (x != z)
        push(x);

    printf("pre traverse bin tree:");
    while (!stack_empty()) {
        x = pop();
        printf("%3d ", x->key);
        if (pre_index)
            *pre_index++ = x->info;

        if (x->r != z) push(x->r);
        if (x->l != z) push(x->l);
    }
    printf("\n");
}

//非递归后序遍历
void post_bin_tree()
{
    struct node* x = head->r;
    init_stack();

    printf("post traverse bin tree:");
    while (x!=z || !stack_empty()) {
        while (x != z) {
            push(x);
            x = x->l;
        }

        x = pop();
        if (x->visited) {
            printf("%3d ", x->key);
            x = z;
        } else {
            x->visited = TRUE;
            push(x);
            x = x->r;
        }
    }
    printf("\n");
}

//层序遍历
void level_bin_tree()
{
    struct node* x = head->r;
    init_queue();

    if (x != z)
        put(x);

    printf("level traverse bin tree:");
    while (!queue_empty()) {
        get(&x);
        printf("%3d ", x->key);
        if (x->l != z) put(x->l);
        if (x->r != z) put(x->r);
    }
    printf("\n");
}

//恢复二叉树
struct node* recovery_bin_tree_pre_in(int *pre, int *in, int N, int *data)
{
    struct node* x;
    struct node *l, *r;

    if (N == 0)
        return z;

    int i;
    x = (struct node*)calloc(1, sizeof(struct node));
    x->info = pre[0];
    x->key = data[x->info];
    x->l = x->r = z;

    for (i = 0; i < N; ++i) {
        if (pre[0] == in[i])
            break;
    }

    l = recovery_bin_tree_pre_in(pre+1, in, i, data);
    r = recovery_bin_tree_pre_in(pre+i+1, in+i+1, N-i-1, data);

    x->l = l;
    x->r = r;

    return x;
}

int search_bin_tree(int v)
{
   z->key = v;
   struct node* x = head->r;

   while (v != x->key) {
       x = v < x->key ? x->l : x->r;
   }
   return x->info;
}

void delete_bin_tree(int v)
{
    z->key = v;
    struct node* x = head->r;
    struct node* p = head->r, *t;

    while (v != x->key) {
        p = x;
        x = v < x->key ? x->l : x->r;
    }

    if (x == z)
        return;
    else if (p == x) {
        free(x);
        head->r = z;
        return;
    }

    t = x;
    if (x->r == z) {            //首先判断右节点是否为空
        x = x->l;
    } else if (x->r->l ==z) {   //再判断右节点的左节点是否为空
        x->r->l = x->l;
        x = x->r;
    } else {                    //最后找出右子树里最小的元素
        x = x->r;
        while (x->l->l != z) {
            x = x->l;
        }
        struct node* tmp = x->l;
        x->l = tmp->r;
        tmp->l = t->l;
        tmp->r = t->r;
        x = tmp;
    }

    free(t);
    if (v < p->key)
        p->l = x;
    else
        p->r = x;
}

struct node_info {
    struct node *pnode;
    int level;
    int pos;
    struct node_info *next;
};

#define MAX_HEIGHT 10
#define MAX_NODE_NUM 100

#define SHOW_RATE 2

void dump_tree(void* arg)
{
    struct node* x = (struct node*)arg;
    struct node_info* levels[MAX_HEIGHT] = {0};
    struct node_info **ppl, *pl;
    int level = 0, child_level;
    struct node_info *info = NULL;
    struct node_info **ppinfo;

    struct node_info* queue[MAX_NODE_NUM] = {0};
    struct node_info **tail, **head;
    tail = head = queue;

    info = calloc(1, sizeof(struct node_info));
    info->level = 0;
    info->pnode = x;
    info->pos = 0;

    levels[0] = info;
    *tail++ = info;

    struct node* t;
    for (level = 0; level < MAX_HEIGHT; ++level) {
        if (levels[level] == NULL)
            break;

        for (child_level = level+1, pl = levels[level]; pl; pl = pl->next) {
            t = pl->pnode;
            ppl = levels + child_level;

            if (t->l != z) {
                info = calloc(1, sizeof(struct node_info));
                info->level = child_level;
                info->pnode = t->l;
                info->pos = pl->pos;

                for (ppinfo = head; ppinfo != tail; ++ppinfo) {
                    if ((*ppinfo)->pos >= info->pos)
                        (*ppinfo)->pos++;
                }

                while (*ppl) {
                    ppl = &(*ppl)->next;
                }
                *ppl = info;
                *tail++ = info;
            }

            if (t->r != z) {
                info = calloc(1, sizeof(struct node_info));
                info->level = child_level;
                info->pnode = t->r;
                info->pos = pl->pos+1;


                for (ppinfo = head; ppinfo != tail; ++ppinfo) {
                    if ((*ppinfo)->pos >= info->pos)
                        (*ppinfo)->pos++;
                }

                while (*ppl) {
                    ppl = &(*ppl)->next;
                }

                *ppl = info;
                *tail++ = info;
            }
        }
    }

    int i;
    int space = 0;
    for (i = 0; i < level; ++i) {
        for (space =0, pl = levels[i]; pl; pl = pl->next) {
            if (pl->pos-space > 0) {
                printf("%*c", (pl->pos - space)*SHOW_RATE , ' ');
                space = pl->pos;
            }

            printf("%-*d", 1, pl->pnode->key);
            ++space;
        }
        printf("\n");
    }

    for (ppinfo = head; ppinfo != tail; ++ppinfo) {
        free(*ppinfo);
    }
}


void test_bin_tree_search()
{
    int data[] = {1, 33, 2, 190, 8, 102, 33, 76, 28, 9, 9, 95, 27};
    int index;
    int i;

    init_bin_tree();
    for (i = 0; i < ARRAY_SIZE(data); ++i) {
        insert_bin_tree(data[i], i);
    }


    printf("search_bin_tree:\n");
    index = search_bin_tree(76);

    printf("index = %d, value = %d\n", index, index>=0 ? data[index] : -1);

    int *pre_index = (int*)calloc(sizeof(int), ARRAY_SIZE(data));
    int *in_index = (int*)calloc(sizeof(int), ARRAY_SIZE(data));


    pre_bin_tree(pre_index);

    in_bin_tree(in_index);

    printf("\nnew tree begin:\n");
    struct node *new_tree = recovery_bin_tree_pre_in(pre_index, in_index, ARRAY_SIZE(data), data);
    struct node* old_tree = head->r;
    head->r = new_tree;

    //
    in_bin_tree(NULL);

    destroy_bin_tree();
    free(pre_index);
    free(in_index);
    head->r = old_tree;
    printf("new tree end.\n");

    printf("test delete bin tree\n");
    delete_bin_tree(9);
    delete_bin_tree(33);
    delete_bin_tree(28);
    delete_bin_tree(102);
    delete_bin_tree(33);
    delete_bin_tree(190);
    delete_bin_tree(76);
    delete_bin_tree(95);
    delete_bin_tree(27);
    delete_bin_tree(1);
    delete_bin_tree(2);
    delete_bin_tree(8);
    delete_bin_tree(9);

    pre_bin_tree(NULL);
    in_bin_tree(NULL);

    insert_bin_tree(69, 5);
    insert_bin_tree(664, 3);
    insert_bin_tree(87, 3);
    insert_bin_tree(3, 6);
    insert_bin_tree(5, 6);
    insert_bin_tree(87, 6);
    insert_bin_tree(9, 6);
    insert_bin_tree(9, 6);


    printf("test dump tree:\n");
    dump_tree(head->r);

    post_bin_tree();

    level_bin_tree();


    destroy_bin_tree();
    free(head);

    /////////////
//    int size = 1024 *1024 * 10 ;
//    int *test_data = malloc(size * sizeof(int));
//    int i;

//    test_data[0] = test_data[1] = 1;
//    for (i = 2; i < size; ++i) {
//        test_data[i] = test_data[i-2] + test_data[i-1];
//    }
////    print_array(test_data, 100);

//    quick_sort(test_data, size);
//    int test_index = select_kth(test_data, size, size-200);
//    printf("test_index = %#x\n", test_index);

//    BenchMark b;
//    benchmark_begin(&b);

//    index = bin_search_func(test_data, size, test_data[test_index]);

//    benchmark_end(&b, "bin_search");

//    printf("index = %d, value = %d\n", index, index>=0 ? test_data[index] : -1);

//    free(test_data);
}


//////////////////////////////////////////////
void test_elementary_search()
{
//    test_seq_search();

//    test_bin_search();


    test_bin_tree_search();

}


////////////// red-black tree //////////////////////////
void init_rbtree()
{
    z = (struct node*)calloc(1, sizeof(struct node));
    z->l = z; z->r = z; z->red = 0; z->info = -1;

    head = (struct node*)calloc(1, sizeof(struct node));
    head->r = z; head->key = 0; head->red = 0;
}

void destroy_rbtree()
{
    destroy_bin_tree();
}

//旋转无非是3-nodes的两个key节点你上我下的调整，同时把子树转移到另一个key节点,
//大小处在连个key节点之间子树可以挂在任意其中一个key节点上。
static struct node* rotate(int v, struct node *y)
{
    struct node *c, *gc;

    if (v < y->key) {
        c = y->l;
    } else {
        c = y->r;
    }

    if (v < c->key) {
        gc = c->l;
        c->l = gc->r;
        gc->r = c;
    } else {
        gc = c->r;
        c->r = gc->l;
        gc->l = c;
    }

    if (v < y->key)
        y->l = gc;
    else
        y->r = gc;

    return gc;
}

static void split(struct node*gg, struct node*g, struct node*p, struct node*x, int v)
{
    x->red = 1;
    x->l->red = 0;
    x->r->red = 0;

    if (p->red) {
        g->red = 1;
        if (v < g->key != v < p->key) p = rotate(v, g);
        x = rotate(v, gg);
        x->red = 0;
    }
    head->r->red = 0;
}

void insert_rbtree(int v, int info)
{
    struct node *x = head, *p = head, *g = head;
    struct node *gg;

    while (x != z) {
        gg = g;
        g = p;
        p = x;
        if (v < x->key) x = x->l;
        else x = x->r;

        //左右都是红树，可把中间的节点提到上一级
        if (x->l->red && x->r->red)
            split(gg, g, p, x, v);
    }

    x = (struct node*)calloc(1, sizeof(struct node));
    x->key = v; x->info = info;
    x->l = z; x->r = z;

    if (v < p->key)
        p->l = x;
    else
        p->r = x;
    split(gg, g, p, x, v);
}

void delete_rbtree(int v)
{
    delete_bin_tree(v);
}


void test_rbtree_search()
{
    init_rbtree();

 //   insert_rbtree(5, 5);

//    insert_bin_tree(69, 5);
//    insert_bin_tree(664, 3);
//    insert_bin_tree(87, 3);
//    insert_bin_tree(3, 6);
//    insert_bin_tree(5, 6);
//    insert_bin_tree(87, 6);
//    insert_bin_tree(9, 6);
//    insert_bin_tree(9, 6);

    insert_rbtree(9, 6);
    insert_rbtree(9, 6);
    insert_rbtree(19, 6);
    insert_rbtree(9, 6);
    insert_rbtree(9, 6);
    insert_rbtree(8, 6);
    insert_rbtree(7, 6);
//    insert_rbtree(9, 6);


    in_bin_tree(NULL);

    printf("dump tree:\n");
    dump_tree(head->r);




    destroy_rbtree();
    free(head);
}












